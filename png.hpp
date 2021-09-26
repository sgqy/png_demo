#pragma once

#include <cstdint>
#include <memory>

class PNG
{
    class PNG_Impl;
    const std::unique_ptr<PNG_Impl> c;

  public:
    // setter
    PNG(const uint16_t w, const uint16_t h);
    ~PNG();
    int import(const void *buf, const uintptr_t len);
    // getter
    const char *data(uintptr_t *len) const;
    const char *fdata(uintptr_t *len) const;
    int write(const char *fn) const;
};