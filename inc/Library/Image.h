#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>

#include "../Container/DataStream.h"
#include "Files.h"
#include "Pixel.h"
#include "Intrinsics.h"

void Image_FlipV(unsigned int* buffer,int width,int height){
    const unsigned int size = sizeof(unsigned int) * width;
    unsigned int* sbuffer = (unsigned int*)malloc(size);
    
    for (int y = 0; y < height / 2; y++) {
        const unsigned int dsti = y * width;
        const unsigned int srci = (height - 1 - y) * width;
        memcpy(sbuffer,buffer + dsti,width * sizeof(unsigned int));
		memcpy(buffer + dsti,buffer + srci,width * sizeof(unsigned int));
		memcpy(buffer + srci,sbuffer,width * sizeof(unsigned int));
	}

    if(sbuffer) free(sbuffer);
}
void Image_FlipH(unsigned int* buffer,int width,int height){
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width / 2; x++) {
            const int sx = width - 1 - x;
            const unsigned int sc = buffer[y * width + x];
            buffer[y * width + x] = buffer[y * width + sx];
            buffer[y * width + sx] = sc;
		}
	}
}
void Image_Swap_RB(unsigned int* buffer,int width,int height){
    //Color* cbuffer = (Color*)buffer;
    //for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
    //        const Color p = cbuffer[y * width + x];
	//		cbuffer[y * width + x].a = p.a;
	//		cbuffer[y * width + x].r = p.b;
	//		cbuffer[y * width + x].g = p.g;
	//		cbuffer[y * width + x].b = p.r;
	//	}
	//}
    Memswap_i32(buffer,0,2,width * height);
}

#define IMAGE_STD
#if defined IMAGE_STD 
#if defined __linux__

#include <png.h>
#include <jpeglib.h>
//#include <zlib.h>
// -lz -lpng -ljpeg

typedef struct {
    const unsigned char* data;
    size_t size;
    size_t offset;
} PngMemReader;

void Png_Read(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count) {
    PngMemReader* reader = (PngMemReader*)png_get_io_ptr(png_ptr);
    if (reader->offset + byte_count > reader->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out_bytes, reader->data + reader->offset, byte_count);
    reader->offset += byte_count;
}
char Png_SaveARGB(const char* filename,unsigned int* buffer,int width,int height) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("[Png]: SaveARGB -> Error fopen: %s\n",filename);
        return 0;
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        printf("[Png]: SaveARGB -> Error write: %s\n",filename);
        fclose(fp);
        return 0;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        printf("[Png]: SaveARGB -> Error info_struct: %s\n",filename);
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("[Png]: SaveARGB -> Error jmpbuf: %s\n",filename);
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return 0;
    }

    png_set_bgr(png);
    png_set_compression_level(png,9);
    png_init_io(png, fp);
    
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);

    png_bytep *rows = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        rows[y] = (png_bytep)&buffer[y * width];
    }

    png_write_image(png, rows);
    png_write_end(png, NULL);

    free(rows);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 1;
}
unsigned int* Png_toARGB(const unsigned char* png_data, size_t png_size, int* width, int* height) {
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    if (!png_image_begin_read_from_memory(&image, png_data, png_size)) {
        fprintf(stderr,"[Png]: toArgb -> Error reading: %s\n",image.message);
        return NULL;
    }

    image.format = PNG_FORMAT_RGBA;
    *width = image.width;
    *height = image.height;

    size_t buffer_size = PNG_IMAGE_SIZE(image);
    unsigned char* rgba_buffer = malloc(buffer_size);
    if (!rgba_buffer) {
        printf("[Png]: toArgb -> Error malloc: %s\n",image.message);
        png_image_free(&image);
        return NULL;
    }

    if (!png_image_finish_read(&image, NULL, rgba_buffer, 0, NULL)) {
        fprintf(stderr,"[Png]: toArgb -> Error decoding: %s\n",image.message);
        free(rgba_buffer);
        png_image_free(&image);
        return NULL;
    }

    unsigned int* argb_buffer = malloc(image.width * image.height * sizeof(unsigned int));
    if (!argb_buffer) {
        free(rgba_buffer);
        png_image_free(&image);
        return NULL;
    }

    for (int i = 0; i < image.width * image.height; i++) {
        unsigned char r = rgba_buffer[i * 4 + 0];
        unsigned char g = rgba_buffer[i * 4 + 1];
        unsigned char b = rgba_buffer[i * 4 + 2];
        unsigned char a = rgba_buffer[i * 4 + 3];
        argb_buffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }

    free(rgba_buffer);
    png_image_free(&image);
    return argb_buffer;
}
unsigned int* Png_LoadToARGB(const unsigned char* png_data, size_t png_size, int* width, int* height) {
    if (!png_data || png_size < 8) {
        printf("[Png]: LoadToARGB -> Invalid input data.\n");
        return NULL;
    }

    // PNG-Signatur prüfen
    if (png_sig_cmp((png_bytep)png_data, 0, 8)) {
        printf("[Png]: LoadToARGB -> Not a valid PNG buffer.\n");
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return NULL;

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("[Png]: LoadToARGB -> Error during decode.\n");
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    // Speicher-Leser einrichten
    PngMemReader reader = { png_data, png_size, 0 };
    png_set_read_fn(png, &reader, Png_Read);

    png_read_info(png, info);

    *width  = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    // Normalisierung (immer RGBA, 8 bit)
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    int w = *width;
    int h = *height;

    png_bytep rgba_data = malloc(w * h * 4);
    if (!rgba_data) {
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    png_bytep* rows = malloc(sizeof(png_bytep) * h);
    if (!rows) {
        free(rgba_data);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    for (int y = 0; y < h; y++) {
        rows[y] = rgba_data + y * w * 4;
    }

    png_read_image(png, rows);
    png_destroy_read_struct(&png, &info, NULL);
    free(rows);

    // RGBA → ARGB
    unsigned int* argb_buffer = malloc(w * h * sizeof(unsigned int));
    if (!argb_buffer) {
        free(rgba_data);
        return NULL;
    }

    for (int i = 0; i < w * h; i++) {
        unsigned char r = rgba_data[i * 4 + 0];
        unsigned char g = rgba_data[i * 4 + 1];
        unsigned char b = rgba_data[i * 4 + 2];
        unsigned char a = rgba_data[i * 4 + 3];
        argb_buffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }

    free(rgba_data);
    return argb_buffer;
}
unsigned int* Png_LoadToARGB_F(const char* filename, int* width, int* height) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        printf("[Png]: LoadToARGB_F -> Error fopen: %s\n", filename);
        return NULL;
    }

    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8 || png_sig_cmp(header, 0, 8)) {
        printf("[Png]: LoadToARGB_F -> Not a valid PNG: %s\n", filename);
        fclose(fp);
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        printf("[Png]: LoadToARGB_F -> Error reading: %s\n", filename);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    png_init_io(png, fp);
    png_set_sig_bytes(png, 8);
    png_read_info(png, info);

    *width  = png_get_image_width(png, info);
    *height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth  = png_get_bit_depth(png, info);

    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    int w = *width;
    int h = *height;

    png_bytep* row_pointers = malloc(sizeof(png_bytep) * h);
    png_bytep rgba_data = malloc(w * h * 4);
    if (!row_pointers || !rgba_data) {
        printf("[Png]: LoadToARGB_F -> Error malloc: %s\n", filename);
        free(row_pointers);
        free(rgba_data);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return NULL;
    }

    for (int y = 0; y < h; y++) {
        row_pointers[y] = rgba_data + (y * w * 4);
    }

    png_read_image(png, row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    free(row_pointers);

    unsigned int* argb_buffer = malloc(w * h * sizeof(unsigned int));
    if (!argb_buffer) {
        free(rgba_data);
        return NULL;
    }

    for (int i = 0; i < w * h; i++) {
        unsigned char r = rgba_data[i * 4 + 0];
        unsigned char g = rgba_data[i * 4 + 1];
        unsigned char b = rgba_data[i * 4 + 2];
        unsigned char a = rgba_data[i * 4 + 3];
        argb_buffer[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }

    free(rgba_data);
    return argb_buffer;
}

char Jpeg_SaveARGB(const char* filename,const unsigned int* argb_buffer,int width,int height,int quality){
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        printf("[Jpeg]: SaveARGB -> Error fopen: %s\n",filename);
        return 0;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;// RGB
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo,quality,TRUE); // quality 0–100
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer;
    unsigned char* row = malloc(width * 3);

    while (cinfo.next_scanline < cinfo.image_height) {
        for (int x = 0; x < width; x++) {
            unsigned int pixel = argb_buffer[cinfo.next_scanline * width + x];
            unsigned char r = (pixel >> 16) & 0xFF;
            unsigned char g = (pixel >> 8) & 0xFF;
            unsigned char b = pixel & 0xFF;

            row[x * 3 + 0] = r;
            row[x * 3 + 1] = g;
            row[x * 3 + 2] = b;
        }

        row_pointer = row;
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    free(row);
    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    return 1;
}
unsigned char* Jpeg_ByARGB(const unsigned int* argb_buffer,int width,int height,int quality,unsigned long* jpeg_size){
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned char* jpeg_buf = NULL;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_mem_dest(&cinfo, &jpeg_buf, jpeg_size);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer;
    unsigned char* row = malloc(width * 3);

    while (cinfo.next_scanline < cinfo.image_height) {
        for (int x = 0; x < width; x++) {
            unsigned int pixel = argb_buffer[cinfo.next_scanline * width + x];
            row[x * 3 + 0] = (pixel >> 16) & 0xFF; // R
            row[x * 3 + 1] = (pixel >> 8) & 0xFF;  // G
            row[x * 3 + 2] = pixel & 0xFF;         // B
        }

        row_pointer = row;
        jpeg_write_scanlines(&cinfo, &row_pointer, 1);
    }

    free(row);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    return jpeg_buf; // Muss mit free() freigegeben werden
}
unsigned int* Jpeg_LoadToARGB(unsigned char* jpeg_data,size_t jpeg_size,int* width,int* height) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, jpeg_data, jpeg_size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    int row_stride = cinfo.output_width * cinfo.output_components;

    unsigned int* argb_buffer = malloc(cinfo.output_width * cinfo.output_height * sizeof(unsigned int));
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    for (int y = 0; y < cinfo.output_height; y++) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int x = 0; x < cinfo.output_width; x++) {
            unsigned char r = buffer[0][x * 3 + 0];
            unsigned char g = buffer[0][x * 3 + 1];
            unsigned char b = buffer[0][x * 3 + 2];
            argb_buffer[y * cinfo.output_width + x] = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return argb_buffer;
}
unsigned int* Jpeg_LoadToARGB_F(const char* filename,int* width,int* height) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("Fehler beim Öffnen der JPEG-Datei");
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    if (size <= 0) {
        fclose(f);
        fprintf(stderr, "Fehler: Datei leer oder ungültig\n");
        return NULL;
    }

    unsigned char* data = malloc(size);
    if (!data) {
        fclose(f);
        fprintf(stderr, "Fehler: Speicher konnte nicht alloziert werden\n");
        return NULL;
    }

    size_t readsize = fread(data, 1, size, f);
    while(readsize <= 0) readsize = fread(data, 1, size, f);
    fclose(f);

    unsigned int* result = Jpeg_LoadToARGB(data, size, width, height);
    free(data);

    return result;
}
#elif defined _WIN32

#include <windows.h>
#include <objbase.h>
#include <wincodec.h>
// #pragma comment(lib, "ole32.lib")
// #pragma comment(lib, "windowscodecs.lib")

static BOOL GuidEqual(REFGUID a, REFGUID b) {
    return memcmp(a, b, sizeof(GUID)) == 0;
}

unsigned int* Jpeg_LoadToARGB(const unsigned char* jpeg_data, size_t jpeg_size, int* width, int* height) {
    if (!jpeg_data || jpeg_size == 0 || !width || !height) return NULL;

    IWICImagingFactory* factory = NULL;
    IWICBitmapDecoder* decoder = NULL;
    IWICBitmapFrameDecode* frame = NULL;
    IWICFormatConverter* converter = NULL;
    IWICStream* wicStream = NULL;
    unsigned int* argb = NULL;

    HRESULT hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                  &IID_IWICImagingFactory, (void**)&factory);
    if (FAILED(hr)) return NULL;

    hr = factory->lpVtbl->CreateStream(factory, &wicStream);
    if (FAILED(hr)) goto cleanup;

    hr = wicStream->lpVtbl->InitializeFromMemory(wicStream, (BYTE*)jpeg_data, (DWORD)jpeg_size);
    if (FAILED(hr)) goto cleanup;

    // Cast zu IStream* (erforderlich bei mingw)
    hr = factory->lpVtbl->CreateDecoderFromStream(factory, (IStream*)wicStream, NULL,
        WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) goto cleanup;

    hr = decoder->lpVtbl->GetFrame(decoder, 0, &frame);
    if (FAILED(hr)) goto cleanup;

    hr = factory->lpVtbl->CreateFormatConverter(factory, &converter);
    if (FAILED(hr)) goto cleanup;

    hr = converter->lpVtbl->Initialize(converter, (IWICBitmapSource*)frame,
        &GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) goto cleanup;

    UINT w, h;
    converter->lpVtbl->GetSize(converter, &w, &h);
    *width = (int)w;
    *height = (int)h;

    argb = (unsigned int*)malloc(w * h * sizeof(unsigned int));
    if (!argb) goto cleanup;

    hr = converter->lpVtbl->CopyPixels(converter, NULL, w * 4, w * h * 4, (BYTE*)argb);
    if (FAILED(hr)) {
        free(argb);
        argb = NULL;
        goto cleanup;
    }

    // BGRA → ARGB
    for (UINT i = 0; i < w * h; i++) {
        unsigned int p = argb[i];
        argb[i] = (p & 0xFF00FF00) | ((p & 0xFF) << 16) | ((p >> 16) & 0xFF);
    }

cleanup:
    if (wicStream) wicStream->lpVtbl->Release(wicStream);
    if (converter) converter->lpVtbl->Release(converter);
    if (frame) frame->lpVtbl->Release(frame);
    if (decoder) decoder->lpVtbl->Release(decoder);
    if (factory) factory->lpVtbl->Release(factory);
    return argb;
}
unsigned int* Jpeg_LoadToARGB_F(const char* filename, int* width, int* height) {
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    if (size <= 0) { fclose(f); return NULL; }

    unsigned char* data = (unsigned char*)malloc(size);
    if (!data) { fclose(f); return NULL; }
    fread(data, 1, size, f);
    fclose(f);

    unsigned int* result = Jpeg_LoadToARGB(data, size, width, height);
    free(data);
    return result;
}

unsigned int* Png_LoadToARGB(const unsigned char* png_data, size_t png_size, int* width, int* height) {
    if (!png_data || png_size == 0 || !width || !height) return NULL;

    IWICImagingFactory* factory = NULL;
    IWICBitmapDecoder* decoder = NULL;
    IWICBitmapFrameDecode* frame = NULL;
    IWICFormatConverter* converter = NULL;
    IWICStream* wicStream = NULL;
    unsigned int* argb = NULL;

    HRESULT hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                  &IID_IWICImagingFactory, (void**)&factory);
    if (FAILED(hr)) return NULL;

    hr = factory->lpVtbl->CreateStream(factory, &wicStream);
    if (FAILED(hr)) goto cleanup;

    hr = wicStream->lpVtbl->InitializeFromMemory(wicStream, (BYTE*)png_data, (DWORD)png_size);
    if (FAILED(hr)) goto cleanup;

    hr = factory->lpVtbl->CreateDecoderFromStream(factory, (IStream*)wicStream, NULL,
        WICDecodeMetadataCacheOnDemand, &decoder);
    if (FAILED(hr)) goto cleanup;

    hr = decoder->lpVtbl->GetFrame(decoder, 0, &frame);
    if (FAILED(hr)) goto cleanup;

    hr = factory->lpVtbl->CreateFormatConverter(factory, &converter);
    if (FAILED(hr)) goto cleanup;

    hr = converter->lpVtbl->Initialize(converter, (IWICBitmapSource*)frame,
        &GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) goto cleanup;

    UINT w, h;
    converter->lpVtbl->GetSize(converter, &w, &h);
    *width = (int)w;
    *height = (int)h;

    argb = (unsigned int*)malloc(w * h * sizeof(unsigned int));
    if (!argb) goto cleanup;

    hr = converter->lpVtbl->CopyPixels(converter, NULL, w * 4, w * h * 4, (BYTE*)argb);
    if (FAILED(hr)) {
        free(argb);
        argb = NULL;
        goto cleanup;
    }

    // BGRA → ARGB
    for (UINT i = 0; i < w * h; i++) {
        unsigned int p = argb[i];
        argb[i] = (p & 0xFF00FF00) | ((p & 0xFF) << 16) | ((p >> 16) & 0xFF);
    }

cleanup:
    if (wicStream) wicStream->lpVtbl->Release(wicStream);
    if (converter) converter->lpVtbl->Release(converter);
    if (frame) frame->lpVtbl->Release(frame);
    if (decoder) decoder->lpVtbl->Release(decoder);
    if (factory) factory->lpVtbl->Release(factory);
    return argb;
}
unsigned int* Png_LoadToARGB_F(const char* filename, int* width, int* height) {
    if (!filename) return NULL;
    FILE* f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    if (size <= 0) { fclose(f); return NULL; }

    unsigned char* data = (unsigned char*)malloc(size);
    if (!data) { fclose(f); return NULL; }
    fread(data, 1, size, f);
    fclose(f);

    unsigned int* result = Png_LoadToARGB(data, size, width, height);
    free(data);
    return result;
}

static HRESULT WIC_SaveImage(const unsigned int* argb_buffer, int width, int height,REFGUID containerFormat, const char* filename, int quality) {
    if (!argb_buffer || width <= 0 || height <= 0 || !filename) return E_INVALIDARG;

    IWICImagingFactory* factory = NULL;
    IWICBitmapEncoder* encoder = NULL;
    IWICBitmapFrameEncode* frame = NULL;
    IWICStream* wicStream = NULL;
    IPropertyBag2* props = NULL;
    unsigned int* bgra = NULL;

    HRESULT hr = CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
                                  &IID_IWICImagingFactory, (void**)&factory);
    if (FAILED(hr)) return hr;

    hr = factory->lpVtbl->CreateStream(factory, &wicStream);
    if (FAILED(hr)) goto cleanup;

    wchar_t wfilename[MAX_PATH] = {0};
    MultiByteToWideChar(CP_UTF8, 0, filename, -1, wfilename, MAX_PATH);

    hr = wicStream->lpVtbl->InitializeFromFilename(wicStream, wfilename, GENERIC_WRITE);
    if (FAILED(hr)) goto cleanup;

    hr = factory->lpVtbl->CreateEncoder(factory, containerFormat, NULL, &encoder);
    if (FAILED(hr)) goto cleanup;

    hr = encoder->lpVtbl->Initialize(encoder, (IStream*)wicStream, WICBitmapEncoderNoCache);
    if (FAILED(hr)) goto cleanup;

    hr = encoder->lpVtbl->CreateNewFrame(encoder, &frame, &props);
    if (FAILED(hr)) goto cleanup;

    if (props && GuidEqual(containerFormat, &GUID_ContainerFormatJpeg)) {
        PROPBAG2 option = {0};
        option.pstrName = (LPOLESTR)L"Quality";
        VARIANT var;
        VariantInit(&var);
        var.vt = VT_UI4;
        var.ulVal = (UINT)quality;
        props->lpVtbl->Write(props, 1, &option, &var);
    }

    hr = frame->lpVtbl->Initialize(frame, props);
    if (FAILED(hr)) goto cleanup;

    hr = frame->lpVtbl->SetSize(frame, (UINT)width, (UINT)height);
    if (FAILED(hr)) goto cleanup;

    GUID pixelFormat = GUID_WICPixelFormat32bppBGRA;
    hr = frame->lpVtbl->SetPixelFormat(frame, &pixelFormat);
    if (FAILED(hr)) goto cleanup;

    bgra = (unsigned int*)malloc((size_t)width * height * sizeof(unsigned int));
    if (!bgra) { hr = E_OUTOFMEMORY; goto cleanup; }

    for (int i = 0; i < width * height; i++) {
        unsigned int p = argb_buffer[i];
        bgra[i] = (p & 0xFF00FF00) | ((p & 0x00FF0000) >> 16) | ((p & 0x000000FF) << 16);
    }

    hr = frame->lpVtbl->WritePixels(frame, (UINT)height, (UINT)width * 4, (UINT)width * height * 4, (BYTE*)bgra);
    if (FAILED(hr)) goto cleanup;

    hr = frame->lpVtbl->Commit(frame);
    if (FAILED(hr)) goto cleanup;

    hr = encoder->lpVtbl->Commit(encoder);

cleanup:
    if (bgra) free(bgra);
    if (props) props->lpVtbl->Release(props);
    if (frame) frame->lpVtbl->Release(frame);
    if (encoder) encoder->lpVtbl->Release(encoder);
    if (wicStream) wicStream->lpVtbl->Release(wicStream);
    if (factory) factory->lpVtbl->Release(factory);
    return hr;
}

char Jpeg_SaveARGB(const char* filename, const unsigned int* argb_buffer, int width, int height, int quality) {
    if (!filename || !argb_buffer || width <= 0 || height <= 0) return 0;

    HRESULT hr = WIC_SaveImage(argb_buffer, width, height, &GUID_ContainerFormatJpeg, filename, quality);
    if (SUCCEEDED(hr)) {
        printf("[Jpeg]: Saved \"%s\" (%dx%d, Quality %d)\n", filename, width, height, quality);
        return 1;
    }
    printf("[Jpeg]: Save failed \"%s\" (0x%08X)\n", filename, hr);
    return 0;
}
char Png_SaveARGB(const char* filename, unsigned int* buffer, int width, int height) {
    if (!filename || !buffer || width <= 0 || height <= 0) return 0;

    HRESULT hr = WIC_SaveImage(buffer, width, height, &GUID_ContainerFormatPng, filename, 100);
    if (SUCCEEDED(hr)) {
        printf("[Png]: Saved \"%s\" (%dx%d)\n", filename, width, height);
        return 1;
    }
    printf("[Png]: Save failed \"%s\" (0x%08X)\n", filename, hr);
    return 0;
}

#elif defined _WEB

#define STB_IMAGE_IMPLEMENTATION
#include "Stb_Image.h"

void Png_Read(void* png_ptr, size_t out_bytes, size_t byte_count) {
    printf("[Png]: Read -> Function not defined!\n");
}
char Png_SaveARGB(const char* filename,unsigned int* buffer,int width,int height) {
    printf("[Png]: SaveARGB -> Function not defined!\n");
    return 0;
}
unsigned int* Png_toARGB(const unsigned char* png_data, size_t png_size, int* width, int* height) {
    printf("[Png]: toARGB -> Function not defined!\n");
    return NULL;
}
unsigned int* Png_LoadToARGB(const unsigned char* png_data, size_t png_size, int* width, int* height) {
    printf("[Png]: LoadToARGB -> Function not defined!\n");
    return NULL;
}
unsigned int* Png_LoadToARGB_F(const char* filename, int* width, int* height) {
    int channels;
    unsigned char* data = stbi_load(filename,width,height,&channels,4);
    return (unsigned int*)data;
}

char Jpeg_SaveARGB(const char* filename,const unsigned int* argb_buffer,int width,int height,int quality){
    printf("[Jpeg]: SaveARGB -> Function not defined!\n");
    return 0;
}
unsigned char* Jpeg_ByARGB(const unsigned int* argb_buffer,int width,int height,int quality,unsigned long* jpeg_size){
    printf("[Jpeg]: ByARGB -> Function not defined!\n");
    return NULL;
}
unsigned int* Jpeg_LoadToARGB(unsigned char* jpeg_data,size_t jpeg_size,int* width,int* height) {
    printf("[Jpeg]: LoadToARGB -> Function not defined!\n");
    return NULL;
}
unsigned int* Jpeg_LoadToARGB_F(const char* filename,int* width,int* height) {
    int channels;
    unsigned char* data = stbi_load(filename,width,height,&channels,4);
    return (unsigned int*)data;
}

#endif

void ARGB_toYUYV(unsigned int* buffer,int width,int height,unsigned char* out,int length) {
    int yuyv_index = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x += 2) {
            unsigned int pixel1 = buffer[y * width + x];
            unsigned int pixel2 = buffer[y * width + (x + 1)];

            //unsigned char a1 = (pixel1 >> 24) & 0xFF;
            unsigned char r1 = (pixel1 >> 16) & 0xFF;
            unsigned char g1 = (pixel1 >> 8) & 0xFF;
            unsigned char b1 = pixel1 & 0xFF;

            //unsigned char a2 = (pixel2 >> 24) & 0xFF;
            unsigned char r2 = (pixel2 >> 16) & 0xFF;
            unsigned char g2 = (pixel2 >> 8) & 0xFF;
            unsigned char b2 = pixel2 & 0xFF;

            // Berechnung von YUYV Werten für zwei Pixel
            unsigned char y1 = (unsigned char)(0.299 * r1 + 0.587 * g1 + 0.114 * b1);
            unsigned char u1 = (unsigned char)(-0.14713 * r1 - 0.28886 * g1 + 0.436 * b1);
            //unsigned char v1 = (unsigned char)(0.615 * r1 - 0.51499 * g1 - 0.10001 * b1);

            unsigned char y2 = (unsigned char)(0.299 * r2 + 0.587 * g2 + 0.114 * b2);
            //unsigned char u2 = (unsigned char)(-0.14713 * r2 - 0.28886 * g2 + 0.436 * b2);
            unsigned char v2 = (unsigned char)(0.615 * r2 - 0.51499 * g2 - 0.10001 * b2);

            // YUYV: Y1 U Y2 V
            out[yuyv_index++] = y1;
            out[yuyv_index++] = u1;
            out[yuyv_index++] = y2;
            out[yuyv_index++] = v2;
        }
    }

    // for (int i = 0; i < width * height; i += 2) {
    //     unsigned char a0 = buffer[i * 4 + 0]; // A
    //     unsigned char r0 = buffer[i * 4 + 1];
    //     unsigned char g0 = buffer[i * 4 + 2];
    //     unsigned char b0 = buffer[i * 4 + 3];
    //     // Pixel 2
    //     unsigned char a1 = buffer[(i + 1) * 4 + 0];
    //     unsigned char r1 = buffer[(i + 1) * 4 + 1];
    //     unsigned char g1 = buffer[(i + 1) * 4 + 2];
    //     unsigned char b1 = buffer[(i + 1) * 4 + 3];
    //     // YUV berechnen
    //     unsigned char y0 = (unsigned char)(0.299 * r0 + 0.587 * g0 + 0.114 * b0);
    //     unsigned char y1 = (unsigned char)(0.299 * r1 + 0.587 * g1 + 0.114 * b1);
    //     unsigned char u  = (unsigned char)(((-0.169 * r0 - 0.331 * g0 + 0.5 * b0) +
    //                             (-0.169 * r1 - 0.331 * g1 + 0.5 * b1)) / 2 + 128);
    //     unsigned char v  = (unsigned char)(((0.5 * r0 - 0.419 * g0 - 0.081 * b0) +
    //                             (0.5 * r1 - 0.419 * g1 - 0.081 * b1)) / 2 + 128);
    //     // In YUYV speichern
    //     out[i * 2 + 0] = y0;
    //     out[i * 2 + 1] = u;
    //     out[i * 2 + 2] = y1;
    //     out[i * 2 + 3] = v;
    // }
}
char Bmp_SaveARGB(const char* filename,unsigned int* buffer,int width,int height) {
    //if(!Files_isFile((char*)filename)) return 0;
    DataStream ds = DataStream_New();
    
    //unsigned char bmpPad[3] = { 0,0,0 };
	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + width * height * sizeof(unsigned int);
	
    DataStream_PushCount(&ds,"BM",2);
    DataStream_PushCount(&ds,(int[]){ fileSize },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ fileHeaderSize + informationHeaderSize },sizeof(int));

    DataStream_PushCount(&ds,(int[]){ informationHeaderSize },sizeof(int));
    DataStream_PushCount(&ds,&width,sizeof(int));
    DataStream_PushCount(&ds,&height,sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0x00200001 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
    DataStream_PushCount(&ds,(int[]){ 0 },sizeof(int));
	
    for (int y = height - 1; y >= 0; y--) {
        const unsigned int dsti = y * width;
        DataStream_PushCount(&ds,buffer + dsti,sizeof(unsigned int) * width);
	}

    Files_Write((char*)filename,ds.Memory,ds.size);
    DataStream_Free(&ds);
    return 1;
}
unsigned int* Bmp_LoadToARGB_F(const char* filename, int* width, int* height) {
    FilesSize fsize;
    char* data = Files_ReadTB((char*)filename,&fsize);
    if (!data) {
        printf("[Bmp]: LoadToARGB_F -> Error fopen: %s\n",filename);
        return NULL;
    }
    DataStream ds = DataStream_By(data,fsize);
    const int size_b = ds.size;
    
    char fileHeader[2];
    DataStream_ReadCount(&ds,fileHeader,0,2);
	
    if (fileHeader[0] != 'B' || fileHeader[1] != 'M') {
		printf("[Bmp]: LoadToARGB_F -> Path '%s' is not a bitmap image!\n",filename);
		return NULL;
	}
    
    int fileSize;
    int fileHISize;
    DataStream_ReadCount(&ds,&fileSize,0,sizeof(int));
    DataStream_ReadCount(&ds,(int[]){ 0 },0,sizeof(int));
    DataStream_ReadCount(&ds,&fileHISize,0,sizeof(int));
    
	int informationHeaderSize; //40 + 20 + 16 * 4
    DataStream_ReadCount(&ds,&informationHeaderSize,0,sizeof(int));
    DataStream_ReadCount(&ds,width,0,sizeof(int));
    DataStream_ReadCount(&ds,height,0,sizeof(int));

    int fileBits;
    DataStream_ReadCount(&ds,&fileBits,0,sizeof(int));
    fileBits >>= 16;

    const int size_r = fileHISize - (size_b - ds.size);
    DataStream_RemoveCount(&ds,0,size_r);

    const unsigned int size = sizeof(unsigned int) * *width * *height;
    unsigned int* argb_buffer = (unsigned int*)malloc(size);
    //const int paddingAmount = ((4 - (*width * 3) % 4) % 4);
    
    if(fileBits == 0x20){ // 32
        DataStream_ReadCount(&ds,argb_buffer,0,size);
    }else if(fileBits == 0x18){ // 24
        unsigned char* ds_m = (unsigned char*)ds.Memory;
        
        for (int y = 0; y < *height; y++) {
            for (int x = 0; x < *width; x++) {
                unsigned int p = 0x0U;
                p |= ds_m[0];
                p |= ds_m[1] << 8;
                p |= ds_m[2] << 16;
                argb_buffer[y * *width + x] = p;
                ds_m += sizeof(unsigned char) * 3;
		    }
	    }
    }else{
        memset(argb_buffer,0,size);
    }

    DataStream_Free(&ds);
    return argb_buffer;
}

#define IMAGE_FLIP_NONE     0b0
#define IMAGE_FLIP_V        0b1
#define IMAGE_FLIP_H        0b10

char Image_FlipState = 0;

void Image_Enable_FlipV(){
    Image_FlipState |= IMAGE_FLIP_V;
}
void Image_Disable_FlipV(){
    Image_FlipState &= ~IMAGE_FLIP_V;
}
void Image_Enable_FlipH(){
    Image_FlipState |= IMAGE_FLIP_H;
}
void Image_Disable_FlipH(){
    Image_FlipState &= ~IMAGE_FLIP_H;
}

char Image_Save(char* filename,unsigned int* buffer,int width,int height) {
    CStr type = Files_Type(filename);
    if(!type) return 0;
    
    if(CStr_Cmp(type,"png") || CStr_Cmp(type,"PNG")){
        CStr_Free(&type);
        return Png_SaveARGB(filename,buffer,width,height);
    }else if(CStr_Cmp(type,"jpg") || CStr_Cmp(type,"jpeg") || CStr_Cmp(type,"JPG") || CStr_Cmp(type,"JPEG")){
        CStr_Free(&type);
        return Jpeg_SaveARGB(filename,buffer,width,height,85);
    }else if(CStr_Cmp(type,"bmp") || CStr_Cmp(type,"BMP")){
        CStr_Free(&type);
        return Bmp_SaveARGB(filename,buffer,width,height);
    }else{
        printf("[Image]: Save -> Error format not valid: %s (%s)\n",type,filename);
        CStr_Free(&type);
        return 0;
    }
}
unsigned int* Image_Load(char* filename,int* width,int* height) {
    CStr type = Files_Type(filename);
    if(!type) return 0;

    if(CStr_Cmp(type,"png") || CStr_Cmp(type,"PNG")){
        CStr_Free(&type);
        unsigned int* buffer = Png_LoadToARGB_F(filename,width,height);
        
        if(Image_FlipState & IMAGE_FLIP_V){
            Image_FlipV(buffer,*width,*height);
        }
        if(Image_FlipState & IMAGE_FLIP_H){
            Image_FlipH(buffer,*width,*height);
        }

        return buffer;
    }else if(CStr_Cmp(type,"jpg") || CStr_Cmp(type,"jpeg") || CStr_Cmp(type,"JPG") || CStr_Cmp(type,"JPEG")){
        CStr_Free(&type);
        unsigned int* buffer = Jpeg_LoadToARGB_F(filename,width,height);

        if(Image_FlipState & IMAGE_FLIP_V){
            Image_FlipV(buffer,*width,*height);
        }
        if(Image_FlipState & IMAGE_FLIP_H){
            Image_FlipH(buffer,*width,*height);
        }
        
        return buffer;
    }else if(CStr_Cmp(type,"bmp") || CStr_Cmp(type,"BMP")){
        CStr_Free(&type);
        unsigned int* buffer = Bmp_LoadToARGB_F(filename,width,height);

        if(!(Image_FlipState & IMAGE_FLIP_V)){
            Image_FlipV(buffer,*width,*height);
        }
        if(Image_FlipState & IMAGE_FLIP_H){
            Image_FlipH(buffer,*width,*height);
        }

        return buffer;
    }else{
        printf("[Image]: Load -> Error format not valid: %s (%s)\n",type,filename);
    }

    CStr_Free(&type);
    return NULL;
}

#else

#define STB_IMAGE_IMPLEMENTATION
#include "Stb_Image.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "Stb_Image_Write.h"

void Image_Enable_FlipV(){
    stbi_set_flip_vertically_on_load(1);
}
void Image_Disable_FlipV(){
    stbi_set_flip_vertically_on_load(0);
}

int Image_Save(const char *filename,unsigned int* buffer,int width,int height) {
    // Save as PNG
    //int channels = 4;
    //if(!stbi_write_png(filename,width,height,channels,buffer,width * channels)) {
    //    printf("[Image]: Save -> Failed to save image.\n");
    //}
    return -1;
}
unsigned int* Image_Load(const char* filename,int* width,int* height) {
    int channels;
    unsigned char* data = stbi_load(filename,width,height,&channels,4);
    return (unsigned int*)data;
}

#endif

#endif // !IMAGE_H