#include "stdafx.hpp"

#include "Logging.hpp"
#include "png.hpp"

#include <libpng/png.h>

class PNG::PNG_Impl
{
    std::vector<char> file;
    std::vector<char> bitmap;
    const uint16_t w;
    const uint16_t h;
    const uint16_t cc; // color count

    void compose()
    {
        png_image img = {
            .version = PNG_IMAGE_VERSION,
            .format  = PNG_FORMAT_BGRA, // notice: endian
            .height  = h,
            .width   = w,
        };
        size_t sz = 0;

        auto ret = png_image_write_to_memory(&img, nullptr, &sz, 0, bitmap.data(), 0, nullptr);
        if (ret == 0)
        {
            LOG_ERRS("{}", img.message);
        }
        file.resize(sz);
        ret = png_image_write_to_memory(&img, file.data(), &sz, 0, bitmap.data(), 0, nullptr);
        if (ret == 0)
        {
            LOG_ERRS("{}", img.message);
        }
    }

  public:
    PNG_Impl(const uint16_t w_, const uint16_t h_) : w(w_), h(h_), cc(4)
    {
    }
    int import(const void *buf, const uintptr_t len)
    {
        LOG_INFO("h: {}, w: {}, color: {}, max: {}, real: {}", h, w, cc, h * w * cc, len);
        if (len < h * w * cc)
        {
            LOG_WARN("data not fill");
        }
        if (len > h * w * cc)
        {
            LOG_WARN("data overflow");
        }
        bitmap.resize(len);
        memcpy(bitmap.data(), buf, len);

        compose();
        return 0; // reserved
    }
    // getter
    const char *data(uintptr_t *len) const
    {
        *len = bitmap.size();
        return bitmap.data();
    }
    const char *fdata(uintptr_t *len) const
    {
        *len = file.size();
        return file.data();
    }
    int write(const char *fn) const
    {
        LOG_INFO("write to file `{}`", fn);
        auto fp = std::unique_ptr<FILE, decltype(&fclose)>(fopen(fn, "wb+"), fclose);
        if (fp == nullptr)
        {
            LOG_ERRS(strerror(errno));
            return -1;
        }
        auto ret = fwrite(file.data(), sizeof(char), file.size(), fp.get());
        LOG_INFO("file written {} bytes", ret);
        if (ret != file.size())
        {
            LOG_ERRS("written byte not match");
            return ret;
        }
        return 0;
    }
};

// interface
PNG::PNG(const uint16_t w, const uint16_t h) : c(new PNG_Impl(w, h))
{
}
PNG::~PNG()
{
}

int PNG::import(const void *buf, const uintptr_t len)
{
    return c->import(buf, len);
}

const char *PNG::data(uintptr_t *len) const
{
    return c->data(len);
}

const char *PNG::fdata(uintptr_t *len) const
{
    return c->fdata(len);
}

int PNG::write(const char *fn) const
{
    return c->write(fn);
}
