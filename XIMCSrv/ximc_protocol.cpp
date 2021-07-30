// kate: replace-tabs off; indent-width 4; indent-mode cstyle; remove-trailing-spaces all; tab-indents on; tab-width 4; indent-width 4
/**
 * (c) 2014 - Michele Devetta (michele.devetta@mail.polimi.it)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ximc_protocol.h"


// Serialization of structure position_t
int XIMCProtocolHandler::serialize(const position_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)POSITION_T_LEN)
		return -1;
	memset(buffer, 0, POSITION_T_LEN);
	SERIALIZE_VAR(buffer, offset, int32_t, obj->position);
	SERIALIZE_VAR(buffer, offset, int16_t, obj->uposition);
	offset += sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Serialization of structure sposition_t
int XIMCProtocolHandler::serialize(const sposition_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)SPOSITION_T_LEN)
		return -1;
	memset(buffer, 0, SPOSITION_T_LEN);
	SERIALIZE_VAR(buffer, offset, int32_t, obj->position);
	SERIALIZE_VAR(buffer, offset, int16_t, obj->uposition);
	SERIALIZE_VAR(buffer, offset, int64_t, obj->enc_position);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->flags);
	offset+=sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Serialization of structure move_settings_t
int XIMCProtocolHandler::serialize(const move_settings_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)MOVE_SETTINGS_T_LEN)
		return -1;
	memset(buffer, 0, MOVE_SETTINGS_T_LEN);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj->speed);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->uspeed);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->accel);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->decel);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj->antiplay_speed);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->antiplay_uspeed);
	offset += sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Serialization of structure feedback_settings_t
int XIMCProtocolHandler::serialize(const feedback_settings_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)FEEDBACK_SETTINGS_T_LEN)
		return -1;
	memset(buffer, 0, FEEDBACK_SETTINGS_T_LEN);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->ips);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->fb_type);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->flags);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->hall_spr);
	SERIALIZE_VAR(buffer, offset, int8_t, obj->hall_shift);
	offset += sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Serialization of structure engine_settings_t
int XIMCProtocolHandler::serialize(const engine_settings_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)ENGINE_SETTINGS_T_LEN)
		return -1;
	memset(buffer, 0, ENGINE_SETTINGS_T_LEN);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->nom_voltage);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->nom_current);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj->nom_speed);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->nom_uspeed);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->flags);
	SERIALIZE_VAR(buffer, offset, int16_t, obj->antiplay);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->microstep_mode);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->steps_per_rev);
	offset += sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Serialization of structure home_settings_t
int XIMCProtocolHandler::serialize(const home_settings_t* obj, uint8_t *buffer, int length) {
	int offset = 0;
	if(length < (int)HOME_SETTINGS_T_LEN)
		return -1;
	memset(buffer, 0, HOME_SETTINGS_T_LEN);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj->fast_home);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->ufast_home);
	SERIALIZE_VAR(buffer, offset, uint32_t, obj->slow_home);
	SERIALIZE_VAR(buffer, offset, uint8_t, obj->uslow_home);
	SERIALIZE_VAR(buffer, offset, int32_t, obj->home_delta);
	SERIALIZE_VAR(buffer, offset, int16_t, obj->uhome_delta);
	SERIALIZE_VAR(buffer, offset, uint16_t, obj->flags);
	offset += sizeof(obj->reserved);
	SERIALIZE_VAR(buffer, offset, uint16_t, this->crc16(buffer, offset));
	return offset;
}

// Un-serialization of structure gposition_t
int XIMCProtocolHandler::unserialize(gposition_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)GPOSITION_T_LEN)
		return -1;
	memset(obj, 0, sizeof(gposition_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+GPOSITION_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, GPOSITION_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, int32_t, obj->position);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->uposition);
	UNSERIALIZE_VAR(buffer, offset, int64_t, obj->enc_position);
	return offset;
}

// Un-serialization of structure status_t
int XIMCProtocolHandler::unserialize(status_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)STATUS_T_LEN)
		return -1;
	memset(obj, 0, sizeof(status_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+STATUS_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, STATUS_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->move_sts);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->mcmd_sts);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->pwr_sts);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->enc_sts);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->wind_sts);
	UNSERIALIZE_VAR(buffer, offset, int32_t, obj->position);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->uposition);
	UNSERIALIZE_VAR(buffer, offset, int64_t, obj->enc_position);
	UNSERIALIZE_VAR(buffer, offset, int32_t, obj->speed);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->uspeed);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->i_pwr);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->v_pwr);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->i_usb);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->v_usb);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->temp);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->flags);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->gpio_flags);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->buff_free);
	return offset;
}

// Un-serialization of structure move_settings_t
int XIMCProtocolHandler::unserialize(move_settings_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)MOVE_SETTINGS_T_LEN)
		return -1;
	memset(obj, 0, sizeof(move_settings_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+MOVE_SETTINGS_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, MOVE_SETTINGS_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->speed);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->uspeed);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->accel);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->decel);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->antiplay_speed);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->antiplay_uspeed);
	return offset;
}

// Un-serialization of structure feedback_settings_t
int XIMCProtocolHandler::unserialize(feedback_settings_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)FEEDBACK_SETTINGS_T_LEN)
		return -1;
	memset(obj, 0, sizeof(feedback_settings_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+FEEDBACK_SETTINGS_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, FEEDBACK_SETTINGS_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->ips);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->fb_type);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->flags);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->hall_spr);
	UNSERIALIZE_VAR(buffer, offset, int8_t, obj->hall_shift);
	return offset;
}

// Un-serialization of structure engine_settings_t
int XIMCProtocolHandler::unserialize(engine_settings_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)ENGINE_SETTINGS_T_LEN)
		return -1;
	memset(obj, 0, sizeof(engine_settings_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+ENGINE_SETTINGS_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, ENGINE_SETTINGS_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->nom_voltage);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->nom_current);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->nom_speed);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->nom_uspeed);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->flags);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->antiplay);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->microstep_mode);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->steps_per_rev);
	return offset;
}

// Un-serialization of structure home_settings_t
int XIMCProtocolHandler::unserialize(home_settings_t* obj, const uint8_t *buffer, int length) {
	if(length < (int)HOME_SETTINGS_T_LEN)
		return -1;
	memset(obj, 0, sizeof(home_settings_t));
	// Check CRC16
	uint16_t checksum = *((uint16_t*)(buffer+HOME_SETTINGS_T_LEN-sizeof(uint16_t)));
	if(this->crc16(buffer, HOME_SETTINGS_T_LEN - sizeof(uint16_t)) != checksum)
		return -1;
	int offset = 0;
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->fast_home);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->ufast_home);
	UNSERIALIZE_VAR(buffer, offset, uint32_t, obj->slow_home);
	UNSERIALIZE_VAR(buffer, offset, uint8_t, obj->uslow_home);
	UNSERIALIZE_VAR(buffer, offset, int32_t, obj->home_delta);
	UNSERIALIZE_VAR(buffer, offset, int16_t, obj->uhome_delta);
	UNSERIALIZE_VAR(buffer, offset, uint16_t, obj->flags);
	return offset;
}

// Compute CRC16
uint16_t XIMCProtocolHandler::crc16(const uint8_t *pbuf, uint16_t length) {
	uint16_t crc, i, j, carry_flag, a;
	crc = 0xffff;
	for(i = 0; i < length; i++) {
		crc = crc ^ pbuf[i];
		for(j = 0; j < 8; j++) {
			a = crc;
			carry_flag = a & 0x0001;
			crc = crc >> 1;
			if(carry_flag == 1) crc = crc ^ 0xa001;
		}
	}
	return crc;
}
