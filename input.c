/* unofficial gameplaySP kai
 *
 * Copyright (C) 2006 Exophase <exophase@gmail.com>
 * Copyright (C) 2007 takka <takka@tfact.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "common.h"

u32 enable_tilt_sensor = 0;
u32 tilt_sensorX = 0x800;
u32 tilt_sensorY = 0x800;

static retro_input_state_t input_state_cb;
void retro_set_input_state(retro_input_state_t cb) { input_state_cb = cb; }

void update_input(void)
{
   unsigned i;
   static uint32_t key = 0;
   uint32_t new_key = 0;

   if (!input_state_cb)
      return;

   for (i = 0; i < sizeof(btn_map) / sizeof(map); i++)
      new_key |= input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, btn_map[i].retropad) ? btn_map[i].gba : 0;

   if ((new_key | key) != key)
   {
     u16 p1_cnt = pIO_REG(REG_P1CNT);

     if (((p1_cnt & 0x4000) != 0) || (reg[CPU_HALT_STATE] == CPU_STOP))
     {
       u16 key_intersection = (p1_cnt & key) & 0x3FF;

       if ((p1_cnt & 0x8000) != 0)
       {
         if (key_intersection == (p1_cnt & 0x3FF))
           pIO_REG(REG_IF) |= IRQ_KEYPAD;
       }
       else
       {
         if (key_intersection != 0)
           pIO_REG(REG_IF) |= IRQ_KEYPAD;
       }
     }
   }

   key = new_key;
   pIO_REG(REG_P1) = (~key) & 0x3FF;
}

