/*
 * vgbuttons.c
 *
 * Created: 03.03.2022 8:39:56
 *  Author: PavlovVG
 */

#include "vgbuttons.h"
#include <stdlib.h>

#define DEBOUNCE_TIME_MS 50

#define INCREMENT_TIME_MS 200

#define MAX_BUTTONS	(uint8_t)7

#define INIT_ERROR	(int8_t)-1
#define INIT_OK		(int8_t)0

int8_t vgbuttons_init(struct vgbuttons *self,
					  uint8_t (*get_state)(void *),
					  void *get_state_context,
					  const vgbuttons_mode_t mode,
					  const uint8_t period_ms)
{
	if (self == NULL || get_state == NULL)
	{
		return INIT_ERROR;
	}
	self->get_state = get_state;
	self->get_state_context = get_state_context;
	self->mode = mode;
	self->period = period_ms;
	self->processing_state = VGBUTTONS_WAITING_FOR_PRESS_STATE;
	self->state = 0;
	self->debounce_ticks = DEBOUNCE_TIME_MS / period_ms;
	self->debounce_counter = self->debounce_ticks;
	self->increment_ticks = INCREMENT_TIME_MS / period_ms;
	self->increment_counter = self->increment_ticks;
	self->increment_boost_counter = 0;
	return INIT_OK;
}

static void debounce(struct vgbuttons *self, const uint8_t new_state)
{
	if (new_state && --self->debounce_counter != 0)
	{
		return;
	}
	if (self->debounce_counter == 0 && new_state == 0)
	{
		self->debounce_counter = self->debounce_ticks;
		self->processing_state = VGBUTTONS_WAITING_FOR_PRESS_STATE;
		return;
	}
	self->debounce_counter = self->debounce_ticks;
	self->state = new_state;
	switch (self->mode)
	{
	case VGBUTTONS_CLICK_MODE:
		self->processing_state = VGBUTTONS_CLICK_STATE;
		break;
	case VGBUTTONS_INCREMENT_MODE:
		self->processing_state = VGBUTTONS_INCREMENT_STATE;
		break;
	default:
		break;
	}
}

static void click_mode(struct vgbuttons *self, const uint8_t new_state)
{
	if (new_state == 0)
	{
		self->processing_state = VGBUTTONS_WAITING_FOR_PRESS_STATE;
	}
}

static void icnrement_mode(struct vgbuttons *self, const uint8_t new_state)
{
	if (new_state == 0)
	{
		self->increment_boost_counter = 0;
		self->increment_counter = self->increment_ticks;
		self->processing_state = VGBUTTONS_WAITING_FOR_PRESS_STATE;
		return;
	}
	if (new_state && --self->increment_counter != 0)
	{
		return;
	}
	self->state = new_state;
	self->increment_counter = self->increment_ticks;
	self->increment_counter -= self->increment_boost_counter;
	if (self->increment_boost_counter < 10)
	{
		self->increment_boost_counter += 2;
	}
	else if (self->increment_boost_counter < self->increment_ticks - 40)
	{
		self->increment_boost_counter += 10;
	}
}

void vgbuttons_processing(struct vgbuttons *self)
{
	uint8_t new_state = self->get_state(self->get_state_context);
	if (self->processing_state == VGBUTTONS_WAITING_FOR_PRESS_STATE && new_state)
	{
		self->processing_state = VGBUTTONS_WAITING_FOR_DEBOUNCE_STATE;
	}
	if (self->processing_state == VGBUTTONS_WAITING_FOR_PRESS_STATE)
	{
		return;
	}
	if (self->processing_state == VGBUTTONS_WAITING_FOR_DEBOUNCE_STATE)
	{
		debounce(self, new_state);
	}
	if (self->processing_state == VGBUTTONS_CLICK_STATE)
	{
		click_mode(self, new_state);
	}
	if (self->processing_state == VGBUTTONS_INCREMENT_STATE)
	{
		icnrement_mode(self, new_state);
	}
}

bool vgbuttons_is_pressed(struct vgbuttons *self, const uint8_t button)
{
	if (button > MAX_BUTTONS)
	{
		return false;
	}
	if (self->state & (1 << button))
	{
		self->state &= ~(1 << button);
		return true;
	}
	return false;
}
