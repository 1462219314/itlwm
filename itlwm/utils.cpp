/*
* Copyright (C) 2020  钟先耀
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include "itlwm.hpp"
#include <IOKit/IOLib.h>

void* itlwm::
malloc(vm_size_t len, int type, int how)
{
    void* addr = IOMalloc(len + sizeof(vm_size_t));
    if (addr == NULL) {
        return NULL;
    }
    *((vm_size_t*) addr) = len;
    return (void*)((uint8_t*)addr + sizeof(vm_size_t));
}

void itlwm::
free(void* addr)
{
    if (addr == NULL) {
        return;
    }
    void* actual_addr = (void*)((uint8_t*)addr - sizeof(vm_size_t));
    vm_size_t len = *((vm_size_t*) actual_addr);
    IOFree(actual_addr, len + sizeof(vm_size_t));
}

int itlwm::
iwm_send_bt_init_conf(struct iwm_softc *sc)
{
    XYLog("%s\n", __FUNCTION__);
    struct iwm_bt_coex_cmd bt_cmd;
    
    bt_cmd.mode = htole32(IWM_BT_COEX_WIFI);
    bt_cmd.enabled_modules = htole32(IWM_BT_COEX_HIGH_BAND_RET);
    
    return iwm_send_cmd_pdu(sc, IWM_BT_CONFIG, 0, sizeof(bt_cmd),
                            &bt_cmd);
}
