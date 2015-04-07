// Copyright (c) 2015- PPSSPP Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0 or later versions.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official git repository and contact information can be found at
// https://github.com/hrydgard/ppsspp and http://www.ppsspp.org/.


#pragma once

#include "CommonTypes.h"

inline u8 Convert4To8(u8 v)
{
	// Swizzle bits: 00001234 -> 12341234
	return (v << 4) | (v);
}

inline u8 Convert5To8(u8 v)
{
	// Swizzle bits: 00012345 -> 12345123
	return (v << 3) | (v >> 2);
}

inline u8 Convert6To8(u8 v)
{
	// Swizzle bits: 00123456 -> 12345612
	return (v << 2) | (v >> 4);
}

inline u16 RGBA8888toRGB565(u32 px) {
	return ((px >> 3) & 0x001F) | ((px >> 5) & 0x07E0) | ((px >> 8) & 0xF800);
}

inline u16 RGBA8888toRGBA4444(u32 px) {
	return ((px >> 4) & 0x000F) | ((px >> 8) & 0x00F0) | ((px >> 12) & 0x0F00) | ((px >> 16) & 0xF000);
}

inline u16 BGRA8888toRGB565(u32 px) {
	return ((px >> 19) & 0x001F) | ((px >> 5) & 0x07E0) | ((px << 8) & 0xF800);
}

inline u16 BGRA8888toRGBA4444(u32 px) {
	return ((px >> 20) & 0x000F) | ((px >> 8) & 0x00F0) | ((px << 4) & 0x0F00) | ((px >> 16) & 0xF000);
}

inline u16 RGBA8888toRGBA5551(u32 px) {
	return ((px >> 3) & 0x001F) | ((px >> 6) & 0x03E0) | ((px >> 9) & 0x7C00) | ((px >> 16) & 0x8000);
}

inline void ARGB8From4444(u16 c, u32 * dst) {
	*dst = ((c & 0xf) << 4) | (((c >> 4) & 0xf) << 12) | (((c >> 8) & 0xf) << 20) | ((c >> 12) << 28);
}

inline void ARGB8From565(u16 c, u32 * dst) {
	*dst = ((c & 0x001f) << 19) | (((c >> 5) & 0x003f) << 11) | ((((c >> 10) & 0x001f) << 3)) | 0xFF000000;
}

inline void ARGB8From5551(u16 c, u32 * dst) {
	*dst = ((c & 0x001f) << 19) | (((c >> 5) & 0x001f) << 11) | ((((c >> 10) & 0x001f) << 3)) | 0xFF000000;
}

// TODO: Swizzle the texture access instead.
inline u32 RGBA2BGRA(u32 src) {
	const u32 r = (src & 0x000000FF) << 16;
	const u32 ga = src & 0xFF00FF00;
	const u32 b = (src & 0x00FF0000) >> 16;
	return r | ga | b;
}


inline u32 DecodeRGBA4444(u16 src)
{
	const u32 r = (src & 0x000F) << 0;
	const u32 g = (src & 0x00F0) << 4;
	const u32 b = (src & 0x0F00) << 8;
	const u32 a = (src & 0xF000) << 12;

	const u32 c = r | g | b | a;
	return c | (c << 4);
}

inline u32 DecodeRGBA5551(u16 src)
{
	u8 r = Convert5To8((src >> 0) & 0x1F);
	u8 g = Convert5To8((src >> 5) & 0x1F);
	u8 b = Convert5To8((src >> 10) & 0x1F);
	u8 a = (src >> 15) & 0x1;
	a = (a) ? 0xff : 0;
	return (a << 24) | (b << 16) | (g << 8) | r;
}

inline u32 DecodeRGB565(u16 src)
{
	u8 r = Convert5To8((src >> 0) & 0x1F);
	u8 g = Convert6To8((src >> 5) & 0x3F);
	u8 b = Convert5To8((src >> 11) & 0x1F);
	u8 a = 0xFF;
	return (a << 24) | (b << 16) | (g << 8) | r;
}

inline u32 DecodeRGBA8888(u32 src)
{
#if 1
	return src;
#else
	// This is the order of the bits.
	u8 r = src & 0xFF;
	u8 g = (src >> 8) & 0xFF;
	u8 b = (src >> 16) & 0xFF;
	u8 a = (src >> 24) & 0xFF;
	return (a << 24) | (b << 16) | (g << 8) | r;
#endif
}

inline u16 RGBA8888To565(u32 value)
{
	u8 r = value & 0xFF;
	u8 g = (value >> 8) & 0xFF;
	u8 b = (value >> 16) & 0xFF;
	r >>= 3;
	g >>= 2;
	b >>= 3;
	return (u16)r | ((u16)g << 5) | ((u16)b << 11);
}

inline u16 RGBA8888To5551(u32 value)
{
	u8 r = value & 0xFF;
	u8 g = (value >> 8) & 0xFF;
	u8 b = (value >> 16) & 0xFF;
	u8 a = (value >> 24) & 0xFF;
	r >>= 3;
	g >>= 3;
	b >>= 3;
	a >>= 7;
	return (u16)r | ((u16)g << 5) | ((u16)b << 10) | ((u16)a << 15);
}

inline u16 RGBA8888To4444(u32 value)
{
	const u32 c = value >> 4;
	const u16 r = (c >> 0) & 0x000F;
	const u16 g = (c >> 4) & 0x00F0;
	const u16 b = (c >> 8) & 0x0F00;
	const u16 a = (c >> 12) & 0xF000;
	return r | g | b | a;
}
