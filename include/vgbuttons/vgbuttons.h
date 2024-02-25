/*
 * vgbuttons.h
 *
 * Created: 03.03.2022 8:40:09
 *  Author: PavlovVG
 */

#ifndef KEYS_LIB_H_
#define KEYS_LIB_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
	VGBUTTONS_CLICK_MODE,
	VGBUTTONS_INCREMENT_MODE
} vgbuttons_mode_t;

typedef enum
{
	VGBUTTONS_WAITING_FOR_PRESS_STATE,
	VGBUTTONS_WAITING_FOR_DEBOUNCE_STATE,
	VGBUTTONS_CLICK_STATE,
	VGBUTTONS_INCREMENT_STATE
} processing_state_t;

struct vgbuttons
{
	uint8_t (*get_state)(void *);
	void *get_state_context;
	vgbuttons_mode_t mode;
	uint8_t period;
	processing_state_t processing_state;
	uint8_t state;
	uint8_t debounce_ticks;
	uint8_t debounce_counter;
	uint16_t increment_ticks;
	uint16_t increment_counter;
	uint16_t increment_boost_counter;
};

int8_t vgbuttons_init(struct vgbuttons *self,
					  uint8_t (*get_state)(void *),
					  void *get_state_context,
					  const vgbuttons_mode_t mode,
					  const uint8_t period_ms);

void vgbuttons_processing(struct vgbuttons *self);

bool vgbuttons_is_pressed(struct vgbuttons *self, const uint8_t button);

#endif /* KEYS_LIB_H_ */
