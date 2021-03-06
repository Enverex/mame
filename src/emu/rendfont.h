// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/***************************************************************************

    rendfont.h

    Rendering system font management.

***************************************************************************/

#ifndef __RENDFONT_H__
#define __RENDFONT_H__

#include "render.h"

// forward instead of include
class osd_font;

//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************


// ======================> render_font

// a render_font describes and provides an interface to a font
class render_font
{
	friend class render_manager;
	friend resource_pool_object<render_font>::~resource_pool_object();

	// construction/destruction
	render_font(render_manager &manager, const char *filename);
	virtual ~render_font();

public:
	// getters
	render_manager &manager() const { return m_manager; }

	// size queries
	int32_t pixel_height() const { return m_height; }
	float char_width(float height, float aspect, char32_t ch);
	float string_width(float height, float aspect, const char *string);
	float utf8string_width(float height, float aspect, const char *utf8string);

	// texture/bitmap queries
	render_texture *get_char_texture_and_bounds(float height, float aspect, char32_t ch, render_bounds &bounds);
	void get_scaled_bitmap_and_bounds(bitmap_argb32 &dest, float height, float aspect, char32_t chnum, rectangle &bounds);

private:
	// a glyph describes a single glyph
	class glyph
	{
	public:
		glyph()
			: width(0),
				xoffs(0), yoffs(0),
				bmwidth(0), bmheight(0),
				rawdata(nullptr),
				texture(nullptr) { }

		int32_t               width;              // width from this character to the next
		int32_t               xoffs, yoffs;       // X and Y offset from baseline to top,left of bitmap
		int32_t               bmwidth, bmheight;  // width and height of bitmap
		const char *        rawdata;            // pointer to the raw data for this one
		render_texture *    texture;            // pointer to a texture for rendering and sizing
		bitmap_argb32       bitmap;             // pointer to the bitmap containing the raw data

		rgb_t               color;

	};

	// internal format
	enum format
	{
		FF_UNKNOWN,
		FF_TEXT,
		FF_CACHED,
		FF_OSD
	};

	// helpers
	glyph &get_char(char32_t chnum);
	void char_expand(char32_t chnum, glyph &ch);
	bool load_cached_bdf(const char *filename);
	bool load_bdf();
	bool load_cached(emu_file &file, uint32_t hash);
	bool load_cached_cmd(emu_file &file, uint32_t hash);
	bool save_cached(const char *filename, uint32_t hash);

	void render_font_command_glyph();

	// internal state
	render_manager &    m_manager;
	format              m_format;           // format of font data
	int                 m_height;           // height of the font, from ascent to descent
	int                 m_yoffs;            // y offset from baseline to descent
	float               m_scale;            // 1 / height precomputed
	glyph               *m_glyphs[17*256];  // array of glyph subtables
	std::vector<char>   m_rawdata;          // pointer to the raw data for the font
	uint64_t              m_rawsize;          // size of the raw font data
	std::unique_ptr<osd_font> m_osdfont;    // handle to the OSD font

	int                 m_height_cmd;       // height of the font, from ascent to descent
	int                 m_yoffs_cmd;        // y offset from baseline to descent
	EQUIVALENT_ARRAY(m_glyphs, glyph *) m_glyphs_cmd; // array of glyph subtables
	std::vector<char>   m_rawdata_cmd;      // pointer to the raw data for the font

	// constants
	static const int CACHED_CHAR_SIZE       = 12;
	static const int CACHED_HEADER_SIZE     = 16;
	static const uint64_t CACHED_BDF_HASH_SIZE   = 1024;
};

void convert_command_glyph(std::string &s);

#endif  /* __RENDFONT_H__ */
