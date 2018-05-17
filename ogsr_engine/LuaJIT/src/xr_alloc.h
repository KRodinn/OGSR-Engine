#ifndef _XR_POOL_H
#define _XR_POOL_H

#include <stddef.h>
#include <windows.h>

typedef long(*PNTAVM)(HANDLE handle, void **addr, ULONG zbits,
    size_t *size, ULONG alloctype, ULONG prot);

extern PNTAVM ntavm;

// В сталкере x64 беда с luajit`ом - из-за того, что луаджит непременно требуется память из младших адресов,
// на больших локациях луаджит не может выделить память, так как она уже занята под другие ресурсы игры.
// Как вариант попробую выделить большой кусок памяти(128МБ) в начале игры и буду его потихоньку выдавать луаджиту

void XR_INIT();
void* XR_MMAP(size_t size);
void XR_DESTROY(void* p, size_t size);

#endif