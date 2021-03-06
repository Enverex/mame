// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    Driver for Midway Zeus games

**************************************************************************/
#include "video/zeus2.h"

class midzeus2_state : public midzeus_state
{
public:
	midzeus2_state(const machine_config &mconfig, device_type type, const char *tag)
				: midzeus_state(mconfig, type, tag), m_zeus(*this, "zeus2") { }
	required_device<zeus2_device> m_zeus;

	DECLARE_WRITE_LINE_MEMBER(zeus_irq);
	DECLARE_VIDEO_START(midzeus2);
	uint32_t screen_update_midzeus2(screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect);
	DECLARE_READ32_MEMBER( zeus2_r );
	DECLARE_WRITE32_MEMBER( zeus2_w );
private:
	TIMER_CALLBACK_MEMBER(int_timer_callback);
	void exit_handler2();
	void zeus2_register32_w(offs_t offset, uint32_t data, bool logit);
	void zeus2_register_update(offs_t offset, uint32_t oldval, bool logit);
	bool zeus2_fifo_process(const uint32_t *data, int numwords);
	void zeus2_pointer_write(uint8_t which, uint32_t value);
	void zeus2_draw_model(uint32_t baseaddr, uint16_t count, bool logit);
	void log_fifo_command(const uint32_t *data, int numwords, const char *suffix);
};
