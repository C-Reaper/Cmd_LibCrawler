#ifndef INTRINSICS_H
#define INTRINSICS_H

#include <immintrin.h>
#include <stdlib.h>
#include <string.h>

void MemshftL(unsigned char *Src, unsigned long long Count, size_t num){
    size_t i = 0;
    for (; i + sizeof(__m256i) <= num; i += sizeof(__m256i)) {
        _mm256_storeu_si256((__m256i *)(Src - Count + i), _mm256_load_si256((__m256i *)(Src + i)));
    }

    for (; i < num; ++i) {
        Src[i - Count] = Src[i];
    }
}
void MemshftR(unsigned char *Src, unsigned long long Count, size_t num){
    size_t i = num - sizeof(__m256i);
    for (; i >= 0; i -= sizeof(__m256i)) {
        _mm256_storeu_si256((__m256i *)(Src + Count + i), _mm256_load_si256((__m256i *)(Src + i)));
    }
    i += sizeof(__m256i) - 1;

    for (; i >= 0; i--) {
        Src[i + Count] = Src[i];
    }
}
void Memxor_i32(unsigned int *Src, unsigned int Xor, size_t num){
    size_t i = 0;
    for (; i + sizeof(__m256i) <= num; i += sizeof(__m256i)) {
        _mm256_storeu_si256((__m256i *)(Src + i), _mm256_xor_si256(_mm256_load_si256((__m256i *)(Src + i)),_mm256_set1_epi32(Xor)));
    }

    for (; i < num; ++i) {
        Src[i] ^= Xor;
    }
}

void Memset_i8(unsigned char *Ptr, unsigned char Value, size_t num) {
    __m256i vec = _mm256_set1_epi8(Value);
    
    size_t i = 0;
    for (; i + 32 <= num; i += 32) {
        _mm256_storeu_si256((__m256i *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_i8(unsigned char *Src, unsigned char *Dst, size_t num) {
    size_t i = 0;
    for (; i + 32 <= num; i += 32) {
        _mm256_storeu_si256((__m256i *)(Dst + i), _mm256_load_si256((__m256i *)(Src + i)));
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}
void Memset_i16(unsigned short *Ptr, unsigned short Value, size_t num) {
    __m256i vec = _mm256_set1_epi16(Value);
    
    size_t i = 0;
    for (; i + 16 <= num; i += 16) {
        _mm256_storeu_si256((__m256i *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_i16(unsigned short *Src, unsigned short *Dst, size_t num) {
    size_t i = 0;
    for (; i + 16 <= num; i += 16) {
        _mm256_storeu_si256((__m256i *)(Dst + i), _mm256_load_si256((__m256i *)(Src + i)));
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}
void Memset_i32(unsigned int *Ptr, unsigned int Value, size_t num) {
    __m256i vec = _mm256_set1_epi32(Value);
    
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_si256((__m256i *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_i32(unsigned int *Src, unsigned int *Dst, size_t num) {
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_si256((__m256i *)(Dst + i), _mm256_loadu_si256((__m256i *)(Src + i)));//_mm256_stream_load_si256
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}
void Memset_i64(unsigned long long *Ptr, unsigned long long Value, size_t num) {
    __m256i vec = _mm256_set1_epi64x(Value);
    
    size_t i = 0;
    for (; i + 4 <= num; i += 4) {
        _mm256_storeu_si256((__m256i *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_i64(unsigned long long *Src, unsigned long long *Dst, size_t num) {
    size_t i = 0;
    for (; i + 4 <= num; i += 4) {
        _mm256_storeu_si256((__m256i *)(Dst + i), _mm256_load_si256((__m256i *)(Src + i)));
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}

void Memset_f32(float *Ptr, float Value, size_t num) {
    __m256 vec = _mm256_set1_ps(Value);
    
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_ps((float *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_f32(float *Src, float *Dst, size_t num) {
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_ps((float *)(Dst + i), _mm256_load_ps((float *)(Src + i)));
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}
void Memset_f64(double *Ptr, double Value, size_t num) {
    __m256d vec = _mm256_set1_pd(Value);
    
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_pd((double *)(Ptr + i), vec);
    }

    for (; i < num; ++i) {
        Ptr[i] = Value;
    }
}
void Memcpy_f64(double *Src, double *Dst, size_t num) {
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        _mm256_storeu_pd((double *)(Dst + i), _mm256_load_pd((double *)(Src + i)));
    }

    for (; i < num; ++i) {
        Dst[i] = Src[i];
    }
}

void Memset_Alpha(unsigned int* Dst,const unsigned int Set, size_t num_pixels) {
    size_t i = 0;
    for (; i + 8 <= num_pixels; i += 8) {
        __m256i Src_pixels = _mm256_set1_epi32(Set);
        __m256i Dest_pixels = _mm256_loadu_si256((__m256i*)(Dst + i));

        __m256 fFull = _mm256_set1_ps(255.0f);
        __m256i Full = _mm256_set1_epi32(0xFF);

        __m256 Src_alpha  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Src_pixels,  24)),fFull);
        __m256 Dest_alpha = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Dest_pixels, 24)),fFull);

        __m256 Part_alpha = _mm256_mul_ps(Dest_alpha,_mm256_sub_ps(_mm256_set1_ps(1.0f),Src_alpha));

        __m256 Out_alpha = _mm256_add_ps(Src_alpha,Part_alpha);

        __m256i Out_red = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 16),Full)),Src_alpha),
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,16),Full)),Part_alpha)),Out_alpha));

        __m256i Out_green = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 8),Full)), Src_alpha),
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,8),Full)),Part_alpha)),Out_alpha));

        __m256i Out_blue = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Src_pixels,Full)), Src_alpha),
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Dest_pixels,Full)),Part_alpha)),Out_alpha));

        __m256i blended_pixels = _mm256_or_si256(Out_blue, 
                                 _mm256_or_si256(_mm256_slli_epi32(Out_green,8),
                                 _mm256_or_si256(_mm256_slli_epi32(Out_red,16),
                                 _mm256_slli_epi32(_mm256_cvttps_epi32(_mm256_mul_ps(Out_alpha,fFull)),24))));

        _mm256_storeu_si256((__m256i*)(Dst + i), blended_pixels);
    }

    for (; i < num_pixels; i++) {
        float Src_alpha = (float)(Set >> 24) / 255.0f;
        float Dest_alpha = (float)(Dst[i] >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((Set >> 16) & 0xFF) * Src_alpha + (float)((Dst[i] >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((Set >> 8) & 0xFF)  * Src_alpha + (float)((Dst[i] >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(Set & 0xFF)         * Src_alpha + (float)(Dst[i] & 0xFF)         * Part_alpha) / Out_alpha);

        Dst[i] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    }
}
void Memcpy_Alpha(const unsigned int* Src,unsigned int* Dest, size_t num_pixels) {
    size_t i = 0;
    for (; i + 8 <= num_pixels; i += 8) {
        __m256i Src_pixels = _mm256_loadu_si256((__m256i*)(Src + i));
        __m256i Dest_pixels = _mm256_loadu_si256((__m256i*)(Dest + i));

        __m256 fFull = _mm256_set1_ps(255.0f);
        __m256i Full = _mm256_set1_epi32(0xFF);

        __m256 Src_alpha  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Src_pixels,  24)),fFull);
        __m256 Dest_alpha = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Dest_pixels, 24)),fFull);

        __m256 Part_alpha = _mm256_mul_ps(Dest_alpha,_mm256_sub_ps(_mm256_set1_ps(1.0f),Src_alpha));

        __m256 Out_alpha = _mm256_add_ps(Src_alpha,Part_alpha);

        __m256i Out_red = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 16),Full)),Src_alpha),
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,16),Full)),Part_alpha)),Out_alpha));

        __m256i Out_green = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 8),Full)), Src_alpha),
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,8),Full)),Part_alpha)),Out_alpha));

        __m256i Out_blue = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Src_pixels,Full)), Src_alpha),
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Dest_pixels,Full)),Part_alpha)),Out_alpha));

        __m256i blended_pixels = _mm256_or_si256(Out_blue, 
                                 _mm256_or_si256(_mm256_slli_epi32(Out_green,8),
                                 _mm256_or_si256(_mm256_slli_epi32(Out_red,16),
                                 _mm256_slli_epi32(_mm256_cvttps_epi32(_mm256_mul_ps(Out_alpha,fFull)),24))));

        _mm256_storeu_si256((__m256i*)(Dest + i), blended_pixels);
    }

    for (; i < num_pixels; i++) {
        float Src_alpha = (float)(Src[i] >> 24) / 255.0f;
        float Dest_alpha = (float)(Dest[i] >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((Src[i] >> 16) & 0xFF) * Src_alpha + (float)((Dest[i] >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((Src[i] >> 8) & 0xFF)  * Src_alpha + (float)((Dest[i] >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(Src[i] & 0xFF)         * Src_alpha + (float)(Dest[i] & 0xFF)         * Part_alpha) / Out_alpha);

        Dest[i] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    }
}
void Memcpy_Alpha_Tint(const unsigned int* Src,unsigned int* Dest, size_t num_pixels,unsigned int Tint) {
    size_t i = 0;
    for (; i + 8 <= num_pixels; i += 8) {
        __m256i Src_pixels = _mm256_loadu_si256((__m256i*)(Src + i));
        __m256i Dest_pixels = _mm256_loadu_si256((__m256i*)(Dest + i));

        __m256 fFull = _mm256_set1_ps(255.0f);
        __m256i Full = _mm256_set1_epi32(0xFF);

        __m256 Src_alpha  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Src_pixels,  24)),fFull);
        __m256 Dest_alpha = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Dest_pixels, 24)),fFull);

        __m256 Part_alpha = _mm256_mul_ps(Dest_alpha,_mm256_sub_ps(_mm256_set1_ps(1.0f),Src_alpha));

        __m256 Out_alpha = _mm256_add_ps(Src_alpha,Part_alpha);
        
        __m256 Tint_red   = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(_mm256_set1_epi32(Tint), 16),Full)),_mm256_set1_ps(255.0f));
        __m256 Tint_green = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(_mm256_set1_epi32(Tint), 8),Full)),_mm256_set1_ps(255.0f));
        __m256 Tint_blue  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_set1_epi32(Tint),Full)),_mm256_set1_ps(255.0f));

        __m256i Out_red = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                          _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 16),Full)),Tint_red),Src_alpha),
                          _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,16),Full)),Part_alpha)),Out_alpha));

        __m256i Out_green = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                            _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 8),Full)),Tint_green), Src_alpha),
                            _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,8),Full)),Part_alpha)),Out_alpha));

        __m256i Out_blue = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                           _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Src_pixels,Full)),Tint_blue), Src_alpha),
                           _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(Dest_pixels,Full)),Part_alpha)),Out_alpha));

        __m256i blended_pixels = _mm256_or_si256(Out_blue, 
                                 _mm256_or_si256(_mm256_slli_epi32(Out_green,8),
                                 _mm256_or_si256(_mm256_slli_epi32(Out_red,16),
                                 _mm256_slli_epi32(_mm256_cvttps_epi32(_mm256_mul_ps(Out_alpha,fFull)),24))));

        _mm256_storeu_si256((__m256i*)(Dest + i), blended_pixels);
    }

    for (; i < num_pixels; i++) {
        float Src_alpha = (float)(Src[i] >> 24) / 255.0f;
        float Dest_alpha = (float)(Dest[i] >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((Src[i] >> 16) & 0xFF) * Src_alpha * (float)((Tint >> 16) & 0xFF) / 255.0f + (float)((Dest[i] >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((Src[i] >> 8) & 0xFF)  * Src_alpha * (float)((Tint >> 8) & 0xFF)  / 255.0f + (float)((Dest[i] >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(Src[i] & 0xFF)         * Src_alpha * (float)((Tint & 0xFF))       / 255.0f + (float)(Dest[i] & 0xFF)         * Part_alpha) / Out_alpha);

        Dest[i] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    }
}

void Memcpy_By_f32(float* r,float* g,float* b,unsigned int* Dest, size_t num_pixels) {
    const __m256 scale = _mm256_set1_ps(255.0f);
    const unsigned char A = 255;

    size_t i = 0;
    for (; i + 7 < num_pixels; i += 8) {
        __m256 r_ps = _mm256_loadu_ps(&r[i]);
        __m256 g_ps = _mm256_loadu_ps(&g[i]);
        __m256 b_ps = _mm256_loadu_ps(&b[i]);

        r_ps = _mm256_mul_ps(r_ps, scale);
        g_ps = _mm256_mul_ps(g_ps, scale);
        b_ps = _mm256_mul_ps(b_ps, scale);

        __m256i r_epi32 = _mm256_cvtps_epi32(r_ps);
        __m256i g_epi32 = _mm256_cvtps_epi32(g_ps);
        __m256i b_epi32 = _mm256_cvtps_epi32(b_ps);

        int r_array[8], g_array[8], b_array[8];
        _mm256_storeu_si256((__m256i*)r_array, r_epi32);
        _mm256_storeu_si256((__m256i*)g_array, g_epi32);
        _mm256_storeu_si256((__m256i*)b_array, b_epi32);

        for (int j = 0; j < 8; ++j) {
            unsigned char R = (unsigned char)r_array[j];
            unsigned char G = (unsigned char)g_array[j];
            unsigned char B = (unsigned char)b_array[j];
            Dest[i + j] = (A << 24) | (R << 16) | (G << 8) | B;
        }
    }

    for (; i < num_pixels; ++i) {
        unsigned char R = (unsigned char)(r[i] * 255.0f);
        unsigned char G = (unsigned char)(g[i] * 255.0f);
        unsigned char B = (unsigned char)(b[i] * 255.0f);
        Dest[i] = (A << 24) | (R << 16) | (G << 8) | B;
    }
}
void Memcpy_G_By_f32(float* gray,unsigned int* Dest, size_t num_pixels) {
    const __m256 scale = _mm256_set1_ps(255.0f);
    const unsigned char A = 255;
    
    size_t i = 0;
    for (; i + 7 < num_pixels; i += 8) {
        __m256 g_ps = _mm256_loadu_ps(&gray[i]);

        g_ps = _mm256_mul_ps(g_ps, scale);

        __m256i g_epi32 = _mm256_cvtps_epi32(g_ps);

        int g_array[8];
        _mm256_storeu_si256((__m256i*)g_array, g_epi32);

        for (int j = 0; j < 8; ++j) {
            unsigned char G = (unsigned char)g_array[j];
            Dest[i + j] = (A << 24) | (G << 16) | (G << 8) | G;
        }
    }

    for (; i < num_pixels; ++i) {
        unsigned char G = (unsigned char)(gray[i] * 255.0f);
        Dest[i] = (A << 24) | (G << 16) | (G << 8) | G;
    }
}

void Memswap_i32(unsigned int *Dst, unsigned int Byte1, unsigned int Byte2, size_t num) {
    unsigned int Mask1 = 0xFF << (Byte1 * 8);
    unsigned int Mask2 = 0xFF << (Byte2 * 8);
    unsigned int Mask = ~(Mask1 ^ Mask2);
    size_t i = 0;
    for (; i + 8 <= num; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i *)(Dst + i));
        __m256i r = _mm256_and_si256(a,_mm256_set1_epi32(Mask));
        __m256i p1 = _mm256_slli_epi32(_mm256_and_si256(a,_mm256_set1_epi32(Mask1)),(Byte2-Byte1) * 8);
        __m256i p2 = _mm256_srli_epi32(_mm256_and_si256(a,_mm256_set1_epi32(Mask2)),(Byte2-Byte1) * 8);
        a = _mm256_or_si256(r,_mm256_or_si256(p1,p2));
        _mm256_storeu_si256((__m256i *)(Dst + i), a);
    }

    for (; i < num; ++i) {
        unsigned int r = Dst[i] & Mask;
        unsigned int p1 = (Dst[i] & Mask1) << ((Byte2-Byte1) * 8);
        unsigned int p2 = (Dst[i] & Mask2) >> ((Byte2-Byte1) * 8);
        Dst[i] = r | p1 | p2;
    }
}
void Memexpand(unsigned int *Src,unsigned int *Dst,int SrcSize,int DstSize) {
    if(SrcSize==DstSize){
        memcpy(Dst,Src,SrcSize * sizeof(unsigned int));
        return;
    }
    
    float a = (float)SrcSize / (float)DstSize;
    int i = 0;

    __m256 scale = _mm256_set1_ps(a);

    for (; i + 8 <= DstSize; i += 8) {
        __m256 idx_f = _mm256_set_ps(
            (float)(i + 7), (float)(i + 6), (float)(i + 5), (float)(i + 4),
            (float)(i + 3), (float)(i + 2), (float)(i + 1), (float)(i + 0)
        );

        __m256 scaled = _mm256_mul_ps(idx_f, scale);
        __m256i src_indices = _mm256_cvttps_epi32(scaled);
        __m256i result = _mm256_i32gather_epi32((const int*)Src, src_indices, sizeof(unsigned int));
        _mm256_storeu_si256((__m256i*)(Dst + i), result);
    }

    for (; i < DstSize; ++i) {
        Dst[i] = Src[(int)((float)i * a)];
    }
}
void Memexpand_G_By_f32(float *Src,unsigned int *Dst,int SrcSize,int DstSize) {
    if(SrcSize==DstSize){
        for (int i = 0; i < DstSize; ++i) {
            const float v = Src[i];
            const unsigned char cv = (unsigned char)(v * 255.0f);
            Dst[i] = cv | (cv << 8U) | (cv << 16U) | (255U << 24U);
        }
    }
    
    float a = (float)SrcSize / (float)DstSize;
    int i = 0;

    /*
    __m256 scale = _mm256_set1_ps(a);

    for (; i + 8 <= DstSize; i += 8) {
        __m256 idx_f = _mm256_set_ps(
            (float)(i + 7), (float)(i + 6), (float)(i + 5), (float)(i + 4),
            (float)(i + 3), (float)(i + 2), (float)(i + 1), (float)(i + 0)
        );

        __m256 scaled = _mm256_mul_ps(idx_f, scale);
        __m256i src_indices = _mm256_cvttps_epi32(scaled);
        __m256i result = _mm256_i32gather_epi32((const int*)Src, src_indices, sizeof(unsigned int));
        _mm256_storeu_si256((__m256i*)(Dst + i), result);
    }
    */

    for (; i < DstSize; ++i) {
        const float v = Src[(int)((float)i * a)];
        const unsigned char cv = (unsigned char)(v * 255.0f);
        Dst[i] = cv | (cv << 8U) | (cv << 16U) | (255U << 24U);
    }
}

void Memexpand_IntShift(unsigned int *Src,unsigned int *Dst,int DstSize,int Shift) {
    size_t i = 0;

    // for (; i + 8 <= DstSize; i += 8) {
    //     __m256i Src_pixels = _mm256_set_epi32(
    //         Src[i >> Shift],    Src[(i+1) >> Shift],Src[(i+2) >> Shift],Src[(i+3) >> Shift],
    //         Src[(i+4) >> Shift],Src[(i+5) >> Shift],Src[(i+6) >> Shift],Src[(i+7) >> Shift]
    //     );
    //     _mm256_storeu_si256((__m256i*)(Dst + i), Src_pixels);
    // }

    if(Shift==0){
        memcpy(Dst,Src,DstSize);
        return;
    }else if(Shift==1){
        for(;i + 2 <= DstSize;i += 2){
            *((long long*)(Dst + i)) = *((long long*)(Src + (i >> Shift)));
        }
    }else if(Shift==2){
        for(;i + 4 <= DstSize;i += 4){
            unsigned int pix = *(Src + (i >> Shift));
            _mm_storeu_si128((__m128i *)(Dst + i), _mm_set1_epi32(pix));
        }
    }else{
        for(;i + 8 <= DstSize;i += 8){
            unsigned int pix = *(Src + (i >> Shift));
            _mm256_storeu_si256((__m256i *)(Dst + i), _mm256_set1_epi32(pix));
        }
    }

    for (;i < DstSize;++i) {
        Dst[i] = Src[i >> Shift];
    }
}
void Memexpand_IntBig(unsigned int *Src,unsigned int *Dst,int DstSize,int Scale) {
    if(Scale==0){
        memcpy(Dst,Src,DstSize);
        return;
    }
    
    size_t i = 0;
    // for (; i + 8 <= DstSize; i += 8) {
    //     __m256i Src_pixels = _mm256_set_epi32(Src[(int)((float)i * a)],     Src[(int)((float)(i+1) * a)],Src[(int)((float)(i+2) * a)],Src[(int)((float)(i+3) * a)],
    //                                           Src[(int)((float)(i+4) * a)], Src[(int)((float)(i+5) * a)],Src[(int)((float)(i+6) * a)],Src[(int)((float)(i+7) * a)]);
    //     _mm256_storeu_si256((__m256i*)(Dst + i), Src_pixels);
    // }

    for (;i < DstSize;++i) {
        Dst[i] = Src[i * Scale];
    }
}
void Memexpand_Alpha(unsigned int *Src,unsigned int *Dst,int SrcSize,int DstSize) {
    float a = (float)SrcSize / (float)DstSize;
    
    size_t i = 0;
    for (; i + 8 <= DstSize; i += 8) {
        __m256i Src_pixels = _mm256_set_epi32(Src[(int)((float)i * a)],     Src[(int)((float)(i+1) * a)],Src[(int)((float)(i+2) * a)],Src[(int)((float)(i+3) * a)],
                                              Src[(int)((float)(i+4) * a)], Src[(int)((float)(i+5) * a)],Src[(int)((float)(i+6) * a)],Src[(int)((float)(i+7) * a)]);
        __m256i Dest_pixels = _mm256_loadu_si256((__m256i*)(Dst + i));

        __m256 fFull = _mm256_set1_ps(255.0f);
        __m256i Full = _mm256_set1_epi32(0xFF);

        __m256 Src_alpha  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Src_pixels,  24)),fFull);
        __m256 Dest_alpha = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Dest_pixels, 24)),fFull);

        __m256 Part_alpha = _mm256_mul_ps(Dest_alpha,_mm256_sub_ps(_mm256_set1_ps(1.0f),Src_alpha));

        __m256 Out_alpha = _mm256_add_ps(Src_alpha,Part_alpha);

        __m256i Out_red = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 16),Full)),Src_alpha),
                          _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,16),Full)),Part_alpha)),Out_alpha));

        __m256i Out_green = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 8),Full)), Src_alpha),
                            _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,8),Full)),Part_alpha)),Out_alpha));

        __m256i Out_blue = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Src_pixels,Full)), Src_alpha),
                           _mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Dest_pixels,Full)),Part_alpha)),Out_alpha));

        __m256i blended_pixels = _mm256_or_si256(Out_blue, 
                                 _mm256_or_si256(_mm256_slli_epi32(Out_green,8),
                                 _mm256_or_si256(_mm256_slli_epi32(Out_red,16),
                                 _mm256_slli_epi32(_mm256_cvttps_epi32(_mm256_mul_ps(Out_alpha,fFull)),24))));

        _mm256_storeu_si256((__m256i*)(Dst + i), blended_pixels);
    }

    for (; i < DstSize; i++) {
        unsigned int SrcPixel = Src[(int)((float)i * a)];
        float Src_alpha = (float)(SrcPixel >> 24) / 255.0f;
        float Dest_alpha = (float)(Dst[i] >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((SrcPixel >> 16) & 0xFF) * Src_alpha + (float)((Dst[i] >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((SrcPixel >> 8) & 0xFF)  * Src_alpha + (float)((Dst[i] >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(SrcPixel & 0xFF)         * Src_alpha + (float)(Dst[i] & 0xFF)         * Part_alpha) / Out_alpha);

        Dst[i] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    }
}
void Memexpand_Alpha_Tint(unsigned int *Src,unsigned int *Dst,int SrcSize,int DstSize,unsigned int Tint) {
    float a = (float)SrcSize / (float)DstSize;
    
    size_t i = 0;
    for (; i + 8 <= DstSize; i += 8) {
        __m256i Src_pixels = _mm256_set_epi32(Src[(int)((float)i * a)],     Src[(int)((float)(i+1) * a)],Src[(int)((float)(i+2) * a)],Src[(int)((float)(i+3) * a)],
                                              Src[(int)((float)(i+4) * a)], Src[(int)((float)(i+5) * a)],Src[(int)((float)(i+6) * a)],Src[(int)((float)(i+7) * a)]);
        __m256i Dest_pixels = _mm256_loadu_si256((__m256i*)(Dst + i));

        __m256 fFull = _mm256_set1_ps(255.0f);
        __m256i Full = _mm256_set1_epi32(0xFF);

        __m256 Src_alpha  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Src_pixels,  24)),fFull);
        __m256 Dest_alpha = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_srli_epi32(Dest_pixels, 24)),fFull);

        __m256 Part_alpha = _mm256_mul_ps(Dest_alpha,_mm256_sub_ps(_mm256_set1_ps(1.0f),Src_alpha));

        __m256 Out_alpha = _mm256_add_ps(Src_alpha,Part_alpha);
        
        __m256 Tint_red   = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(_mm256_set1_epi32(Tint), 16),Full)),_mm256_set1_ps(255.0f));
        __m256 Tint_green = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(_mm256_set1_epi32(Tint), 8),Full)),_mm256_set1_ps(255.0f));
        __m256 Tint_blue  = _mm256_div_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_set1_epi32(Tint),Full)),_mm256_set1_ps(255.0f));

        __m256i Out_red = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                          _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 16),Full)),Tint_red),Src_alpha),
                          _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,16),Full)),Part_alpha)),Out_alpha));

        __m256i Out_green = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                            _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Src_pixels, 8),Full)),Tint_green), Src_alpha),
                            _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(_mm256_srli_epi32(Dest_pixels,8),Full)),Part_alpha)),Out_alpha));

        __m256i Out_blue = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_add_ps(
                           _mm256_mul_ps(_mm256_mul_ps(_mm256_cvtepi32_ps(_mm256_and_si256(Src_pixels,Full)),Tint_blue), Src_alpha),
                           _mm256_mul_ps(              _mm256_cvtepi32_ps(_mm256_and_si256(Dest_pixels,Full)),Part_alpha)),Out_alpha));

        __m256i blended_pixels = _mm256_or_si256(Out_blue, 
                                 _mm256_or_si256(_mm256_slli_epi32(Out_green,8),
                                 _mm256_or_si256(_mm256_slli_epi32(Out_red,16),
                                 _mm256_slli_epi32(_mm256_cvttps_epi32(_mm256_mul_ps(Out_alpha,fFull)),24))));

        _mm256_storeu_si256((__m256i*)(Dst + i), blended_pixels);
    }

    for (; i < DstSize; i++) {
        unsigned int SrcPixel = Src[(int)((float)i * a)];
        float Src_alpha = (float)(SrcPixel >> 24) / 255.0f;
        float Dest_alpha = (float)(Dst[i] >> 24) / 255.0f;

        float Part_alpha = Dest_alpha * (1.0f - Src_alpha);
        float Out_alpha  = (Src_alpha + Part_alpha);

        unsigned int Out_red    = (unsigned int)(((float)((SrcPixel >> 16) & 0xFF) * Src_alpha * (float)((Tint >> 16) & 0xFF) / 255.0f + (float)((Dst[i] >> 16) & 0xFF) * Part_alpha) / Out_alpha);
        unsigned int Out_green  = (unsigned int)(((float)((SrcPixel >> 8) & 0xFF)  * Src_alpha * (float)((Tint >> 8) & 0xFF)  / 255.0f + (float)((Dst[i] >> 8) & 0xFF)  * Part_alpha) / Out_alpha);
        unsigned int Out_blue   = (unsigned int)(((float)(SrcPixel & 0xFF)         * Src_alpha * (float)((Tint & 0xFF))       / 255.0f + (float)(Dst[i] & 0xFF)         * Part_alpha) / Out_alpha);

        Dst[i] = ((unsigned int)(Out_alpha * 255.0f) << 24) | (Out_red << 16) | (Out_green << 8) | Out_blue;
    }
}

void Memxsh_i8(char* Src,char* Dst,unsigned int DstSize,unsigned int Shift) {
    size_t i = 0;

    for (; i + 32 <= DstSize; i += 32) {
        __m256i Src_pixels = _mm256_set_epi8(
            Src[(i+0) >> Shift],Src[(i+1) >> Shift],Src[(i+2) >> Shift],Src[(i+3) >> Shift],
            Src[(i+4) >> Shift],Src[(i+5) >> Shift],Src[(i+6) >> Shift],Src[(i+7) >> Shift],
            Src[(i+8) >> Shift],Src[(i+9) >> Shift],Src[(i+10) >> Shift],Src[(i+11) >> Shift],
            Src[(i+12) >> Shift],Src[(i+13) >> Shift],Src[(i+14) >> Shift],Src[(i+15) >> Shift],
            Src[(i+16) >> Shift],Src[(i+17) >> Shift],Src[(i+18) >> Shift],Src[(i+19) >> Shift],
            Src[(i+20) >> Shift],Src[(i+21) >> Shift],Src[(i+22) >> Shift],Src[(i+23) >> Shift],
            Src[(i+24) >> Shift],Src[(i+25) >> Shift],Src[(i+26) >> Shift],Src[(i+27) >> Shift],
            Src[(i+28) >> Shift],Src[(i+29) >> Shift],Src[(i+30) >> Shift],Src[(i+31) >> Shift]
        );
        _mm256_storeu_si256((__m256i*)(Dst + i), Src_pixels);
    }

    for (;i < DstSize;++i) {
        Dst[i] = Src[i >> Shift];
    }
}
void Memexp_i8(char* Src,char* Dst,int SrcSize,int DstSize) {
    if(SrcSize==DstSize){
        memcpy(Dst,Src,SrcSize * sizeof(char));
        return;
    }
    
    float a = (float)SrcSize / (float)DstSize;
    int i = 0;

    for (; i < DstSize; ++i) {
        Dst[i] = Src[(int)((float)i * a)];
    }
}

void Memxsh_i16(short* Src,short* Dst,unsigned int DstSize,unsigned int Shift) {
    size_t i = 0;

    for (; i + 16 <= DstSize; i += 16) {
        __m256i Src_pixels = _mm256_set_epi16(
            Src[(i+0) >> Shift],Src[(i+1) >> Shift],Src[(i+2) >> Shift],Src[(i+3) >> Shift],
            Src[(i+4) >> Shift],Src[(i+5) >> Shift],Src[(i+6) >> Shift],Src[(i+7) >> Shift],
            Src[(i+8) >> Shift],Src[(i+9) >> Shift],Src[(i+10) >> Shift],Src[(i+11) >> Shift],
            Src[(i+12) >> Shift],Src[(i+13) >> Shift],Src[(i+14) >> Shift],Src[(i+15) >> Shift]
        );
        _mm256_storeu_si256((__m256i*)(Dst + i), Src_pixels);
    }

    for (;i < DstSize;++i) {
        Dst[i] = Src[i >> Shift];
    }
}
void Memexp_i16(short* Src,short* Dst,int SrcSize,int DstSize) {
    if(SrcSize==DstSize){
        memcpy(Dst,Src,SrcSize * sizeof(short));
        return;
    }
    
    float a = (float)SrcSize / (float)DstSize;
    int i = 0;

    for (; i < DstSize; ++i) {
        Dst[i] = Src[(int)((float)i * a)];
    }
}
void Memavg_i16(short* Src,short* Dst,size_t num) {
    size_t i = 0;
    for (; i + 16 <= num; i += 16) {
        const __m256i a = _mm256_load_si256((__m256i *)(Dst + i));
        const __m256i b = _mm256_load_si256((__m256i *)(Src + i));
        const __m256i res = _mm256_srli_epi16(_mm256_add_epi16(a,b),1);
        _mm256_storeu_si256((__m256i *)(Dst + i),res);
    }

    for (; i < num; ++i) {
        Dst[i] = (Dst[i] + Src[i]) >> 1;
    }
}

#endif // INTRINSICS_H