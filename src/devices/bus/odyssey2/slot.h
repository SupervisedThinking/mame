// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
#ifndef MAME_BUS_ODYSSEY2_SLOT_H
#define MAME_BUS_ODYSSEY2_SLOT_H

#pragma once

#include "softlist_dev.h"


/***************************************************************************
 TYPE DEFINITIONS
 ***************************************************************************/


/* PCB */
enum
{
	O2_STD = 0,
	O2_4IN1,
	O2_RALLY,
	O2_KTAA,
	O2_CHESS,
	O2_HOMECOMP,
	O2_VOICE
};


// ======================> device_o2_cart_interface

class device_o2_cart_interface : public device_interface
{
	friend class o2_cart_slot_device;

public:
	// construction/destruction
	virtual ~device_o2_cart_interface();

	// reading and writing
	virtual u8 read_rom04(offs_t offset) { return 0xff; }
	virtual u8 read_rom0c(offs_t offset) { return 0xff; }
	virtual void write_p1(u8 data) { }
	virtual void write_p2(u8 data) { }

	virtual void io_write(offs_t offset, u8 data) { }
	virtual u8 io_read(offs_t offset) { return 0xff; }
	virtual DECLARE_READ_LINE_MEMBER(t0_read) { return 0; }
	virtual int b_read() { return -1; }

	virtual void cart_init() { } // called after loading ROM

	u8* get_rom_base() { return m_rom ? &m_rom[0] : nullptr; }
	u32 get_rom_size() { return m_rom ? m_rom.bytes() : 0; }

	u8* get_voice_base() { return m_voice ? &m_voice[0] : nullptr; }
	u32 get_voice_size() { return m_voice ? m_voice.bytes() : 0; }

protected:
	device_o2_cart_interface(const machine_config &mconfig, device_t &device);

	optional_shared_ptr<u8> m_rom;
	optional_shared_ptr<u8> m_exrom;
	optional_shared_ptr<u8> m_voice;
};


// ======================> o2_cart_slot_device

class o2_cart_slot_device : public device_t,
								public device_image_interface,
								public device_single_card_slot_interface<device_o2_cart_interface>
{
public:
	// construction/destruction
	template <typename T>
	o2_cart_slot_device(machine_config const &mconfig, char const *tag, device_t *owner, T &&opts, char const *dflt)
		: o2_cart_slot_device(mconfig, tag, owner, 0)
	{
		option_reset();
		opts(*this);
		set_default_option(dflt);
		set_fixed(false);
	}

	o2_cart_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock = 0);
	virtual ~o2_cart_slot_device();

	// image-level overrides
	virtual image_init_result call_load() override;
	virtual void call_unload() override { }

	virtual iodevice_t image_type() const noexcept override { return IO_CARTSLOT; }
	virtual bool is_readable()  const noexcept override { return true; }
	virtual bool is_writeable() const noexcept override { return false; }
	virtual bool is_creatable() const noexcept override { return false; }
	virtual bool must_be_loaded() const noexcept override { return true; }
	virtual bool is_reset_on_load() const noexcept override { return true; }
	virtual const char *image_interface() const noexcept override { return "odyssey_cart"; }
	virtual const char *file_extensions() const noexcept override { return "bin,rom"; }

	// slot interface overrides
	virtual std::string get_default_card_software(get_default_card_software_hook &hook) const override;

	int get_type() { return m_type; }
	device_o2_cart_interface* cart() { return m_cart; }

	// reading and writing
	u8 read_rom04(offs_t offset);
	u8 read_rom0c(offs_t offset);
	void io_write(offs_t offset, u8 data);
	u8 io_read(offs_t offset);
	DECLARE_READ_LINE_MEMBER(t0_read) { if (m_cart) return m_cart->t0_read(); else return 0; }
	int b_read();

	void write_p1(u8 data) { if (m_cart) m_cart->write_p1(data); }
	void write_p2(u8 data) { if (m_cart) m_cart->write_p2(data); }

protected:
	// device-level overrides
	virtual void device_start() override;

	// device_image_interface implementation
	virtual const software_list_loader &get_software_list_loader() const override { return rom_software_list_loader::instance(); }

	int m_type;
	device_o2_cart_interface* m_cart;
	int m_b;
};

// device type definition
DECLARE_DEVICE_TYPE(O2_CART_SLOT, o2_cart_slot_device)


void o2_cart(device_slot_interface &device);

#endif // MAME_BUS_ODYSSEY2_SLOT_H
