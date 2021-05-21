#pragma once

//
// Device type           -- in the "User Defined" range."
//
#define FILE_TYPE_GENERAL 0x00004000
//
// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
//
#define IOCTL_FGE_HOOK_SSDT \
    CTL_CODE(FILE_TYPE_GENERAL, 0x100, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_FGE_HOOK_INF \
    CTL_CODE(FILE_TYPE_GENERAL, 0x101, METHOD_BUFFERED, FILE_ANY_ACCESS)

