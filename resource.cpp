/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#include "common/file.h"

#include "deskadv/deskadv.h"
#include "deskadv/resource.h"

namespace Deskadv {

Resource::Resource(DeskadvEngine *vm) : _vm(vm) {
	_file = 0;
	_tileCount = 0;
	_tileDataOffset = 0;
}

Resource::~Resource() {
	while (!_zones.empty()) {
		ZONE *temp = &_zones.back();
		delete[] temp->tiles[0];
		delete[] temp->tiles[1];
		delete[] temp->tiles[2];
		_zones.pop_back();
	}

	if (_file)
		_file->close();

	delete _file;
}

bool Resource::load(const char *filename, bool isYoda) {

	// Multiple calls of load not supported.
	assert(_file == 0);

	_isYoda = isYoda;

	_file = new Common::File();
	if (!_file->open(filename))
		return false;

	uint32 tag;
	do {
		tag = this->readTag();
	} while (!_vm->shouldQuit() && tag != MKTAG('E', 'N', 'D', 'F'));

	return true;
}

uint32 Resource::readTag(void) {
	assert(_file != 0);

	uint32 tag = _file->readUint32BE();
	switch (tag) {
	case MKTAG('V', 'E', 'R', 'S'): {
		uint32 version = _file->readUint32LE();
		debugC(1, kDebugResource, "Version: %d", version);
		if (version != 0x200)
			warning("Unsupported Version");
	}
	break;
	case MKTAG('E', 'N', 'D', 'F'):
		debugC(1, kDebugResource, "End of File");
		break;
	case MKTAG('S', 'T', 'U', 'P'): {
		uint32 size = _file->readUint32LE();
		assert(size == 32 * 32 * 9 * 9);
		_stupOffset = _file->pos();
		_file->seek(size, SEEK_CUR);
	}
	break;
	case MKTAG('S', 'N', 'D', 'S'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		int16 count = _file->readSint16LE();
		assert(count <= 0);
		while (count++) {
			uint16 strsize = _file->readUint16LE();
			Common::String strname;
			// TODO: read sound name in one batch
			char c;
			while ((c = _file->readByte()) != 0 && strname.size() < strsize)
				strname += c;
			debugC(1, kDebugResource, "Sound \"%s\"", strname.c_str());
			_soundFiles.push_back(strname);
		}
	}
	break;
	case MKTAG('Z', 'A', 'U', 'X'): // intentional fallthorugh
	case MKTAG('Z', 'A', 'X', '2'): // intentional fallthorugh
	case MKTAG('Z', 'A', 'X', '3'): // intentional fallthrough
	case MKTAG('Z', 'A', 'X', '4'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		uint32 lastTag = 0;
		for (uint zoneID = 0; zoneID < _zoneCount && !_vm->shouldQuit(); zoneID++) {
			lastTag = this->readTag();
			assert(lastTag == MKTAG('I', 'Z', 'A', 'X') ||
			       lastTag == MKTAG('I', 'Z', 'X', '2') ||
			       lastTag == MKTAG('I', 'Z', 'X', '3') ||
			       lastTag == MKTAG('I', 'Z', 'X', '4'));
		}
	}
	break;
	case MKTAG('I', 'Z', 'A', 'X'): // intentional fallthorugh
	case MKTAG('I', 'Z', 'X', '2'): // intentional fallthrough
	case MKTAG('I', 'Z', 'X', '3'): {
		int size = _file->readUint16LE() - 4 - 2;
		_file->seek(size, SEEK_CUR);
	}
	break;
	case MKTAG('I', 'Z', 'X', '4'):
		_file->seek(6, SEEK_CUR);
		break;
	case MKTAG('C', 'H', 'W', 'P'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		uint16_t index = 0xFFFF;
		while (!_vm->shouldQuit() && (index = _file->readUint16LE()) != 0xFFFF) {
			uint8 weaponData[4];
			_file->read(weaponData, 4);
		}
	}
	break;
	case MKTAG('C', 'A', 'U', 'X'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		uint16_t index = 0xFFFF;
		while (!_vm->shouldQuit() && (index = _file->readUint16LE()) != 0xFFFF) {
			uint8 auxData[2];
			_file->read(auxData, 2);
		}
	}
	break;
	case MKTAG('P', 'N', 'A', 'M'): {
		uint32 size = _file->readUint32LE();
		debugC(1, kDebugResource, "Found %s tag, size %d", tag2str(tag), size);
		uint16 count = _file->readUint16LE();
		for (uint i = 0; i < count; i++) {
			// TODO: read name in one batch
			Common::String name;
			for (uint16 j = 0; j < 16; j++)
				name += _file->readByte();
			name = name.c_str(); // Drop extra trailing nulls
			debugC(1, kDebugResource, "entry %04x (%d) is \"%s\"", i, i,
			       name.c_str());
		}
	}
	break;
	case MKTAG('A', 'N', 'A', 'M'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		while (!_vm->shouldQuit()) {
			uint16 zoneid = _file->readUint16LE();
			if (zoneid == 0xffff)
				break;
			debugC(1, kDebugResource, "entry for zone %04x (%d)", zoneid, zoneid);
			while (!_vm->shouldQuit()) {
				uint16 id = _file->readUint16LE();
				if (id == 0xffff)
					break;
				Common::String name;
				uint16 len = _isYoda ? 24 : 16;
				for (uint16 i = 0; i < len; i++)
					name += _file->readByte();
				name = name.c_str(); // Drop extra trailing nulls
				debugC(1, kDebugResource, "entry id %04x (%d) is \"%s\"", id, id,
				       name.c_str());
			}
		}
	}
	break;
	case MKTAG('T', 'N', 'A', 'M'): { // Tile Names
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		while (!_vm->shouldQuit()) {
			TNAME t;
			t.id = _file->readUint16LE();
			if (t.id == 0xffff)
				break;

			uint16 len = _isYoda ? 24 : 16;
			for (uint16 i = 0; i < len; i++)
				t.name += _file->readByte();
			t.name = t.name.c_str(); // Drop extra trailing nulls
			debugC(1, kDebugResource, "entry id %04x (%d) is \"%s\"", t.id, t.id,
			       t.name.c_str());
			_tileNames.push_back(t);
		}
	}
	break;
	case MKTAG('Z', 'N', 'A', 'M'): { // Zone Names
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		while (!_vm->shouldQuit()) {
			uint16 id = _file->readUint16LE();
			if (id == 0xffff)
				break;
			Common::String name;
			uint16 len = _isYoda ? 24 : 16;
			for (uint16 i = 0; i < len; i++)
				name += _file->readByte();
			name = name.c_str(); // Drop extra trailing nulls
			debugC(1, kDebugResource, "entry id %04x (%d) is \"%s\"", id, id,
			       name.c_str());
		}
	}
	break;
	case MKTAG('C', 'H', 'A', 'R'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;
		uint16 characterIndex = 0xFFFF;
		uint32 lastTag = 0;
		while (!_vm->shouldQuit() &&
		        (characterIndex = _file->readUint16LE()) != 0xFFFF) {
			debugC(1, kDebugResource, "    CHAR index: 0x%02x", characterIndex);
			lastTag = this->readTag();
			assert(lastTag == MKTAG('I', 'C', 'H', 'A'));
		}
	}
	break;
	case MKTAG('I', 'C', 'H', 'A'): {
		uint32 size = _file->readUint32LE();

		Common::String name;
		byte character = 0;
		while ((character = _file->readByte()) != 0)
			name += character;

		debugC(1, kDebugResource, "    CHAR name: \"%s\"", name.c_str());
		const uint unknownDataSize = size - name.size() - 1 - 3 * 8 * 2;
		uint8 *unknownData = new byte[unknownDataSize];
		_file->read(unknownData, unknownDataSize);
		delete[] unknownData;

		uint16 frames[3 * 8];
		_file->read(frames, 3 * 8 * sizeof(uint16));
	}
	break;
	case MKTAG('A', 'C', 'T', 'N'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;

		uint16 zoneId = 0xFFFF;
		while (!_vm->shouldQuit() && (zoneId = _file->readUint16LE()) != 0xFFFF) {
			uint32 lastTag = 0;
			uint16 iactCount = _file->readUint16LE();
			for (uint16 j = 0; j < iactCount; j++) {
				lastTag = this->readTag();
				assert(lastTag == MKTAG('I', 'A', 'C', 'T'));
			}
		}
	}
	break;
	case MKTAG('I', 'A', 'C', 'T'): {
		uint32 ignored6 = _file->readUint32LE();
		uint16 conditionCount = _file->readUint16LE();
		debugC(1, kDebugResource, "  ACTN: condition %08x, count1 %d", ignored6,
		       conditionCount);
		for (uint16 k = 0; k < conditionCount; k++) {
			SCRIPT *s = this->readScript();
			debugC(1, kDebugResource,
			       "   ACTN condition %04x(%04x, %04x, %04x, %04x, %04x, %04x)",
			       s->opcode, s->args[0], s->args[1], s->args[2], s->args[3], s->args[4], s->text != NULL ? (uint16)strlen(s->text) : -1);
			assert(s->opcode < 0x26);
			if (s->text) free(s->text);
			delete s;
		}

		uint16 instructionCount = _file->readUint16LE();
		debugC(1, kDebugResource, "  ACTN: instruction count %d", instructionCount);
		for (uint16 k = 0; k < instructionCount; k++) {
			SCRIPT *s = this->readScript();
			debugC(1, kDebugResource,
			       "   ACTN instruction %04x(%04x, %04x, %04x, %04x, %04x, %04x)",
			       s->opcode, s->args[0], s->args[1], s->args[2], s->args[3], s->args[4], s->text != NULL ? (uint16)strlen(s->text) : -1);
			assert(s->opcode < 0x26);
			if (s->text) free(s->text);
			delete s;
		}
	}
	break;
	case MKTAG('H', 'T', 'S', 'P'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;

		uint16 zoneId = 0xFFFF;
		while (!_vm->shouldQuit() && (zoneId = _file->readUint16LE()) != 0xFFFF) {
			uint16 hotspotCount = _file->readUint16LE();
			debugC(1, kDebugResource, "   %d Hotspots for zone: 0x%04x", hotspotCount,
			       zoneId);
			for (uint i = 0; i < hotspotCount; i++) {
				HOTSPOT *h = this->readHotspot();
				debugC(1, kDebugResource, " zone hotspot data: type %08x, "
				       "unknowns %04x, %04x, %04x, unknown data "
				       "%04x",
				       h->type, h->x, h->y, h->arg1, h->arg2);
				delete h;
			}
		}
	}
	break;
	case MKTAG('P', 'U', 'Z', '2'): {
		uint32 size = _file->readUint32LE();
		debugC(1, kDebugResource, "Found %s tag, size %d", tag2str(tag), size);
		while (!_vm->shouldQuit()) {
			uint16 puzid = _file->readUint16LE();
			if (puzid == 0xffff)
				break;
			tag = _file->readUint32BE();
			assert(tag == MKTAG('I', 'P', 'U', 'Z'));
			uint32 ipuzSize = _file->readUint32LE();
			uint32 u1 = _file->readUint32LE();
			uint32 u2 = _file->readUint32LE();
			uint32 u3 = 0;
			if (_isYoda)
				u3 = _file->readUint32LE();
			uint16 u4 = _file->readUint16LE();
			debugC(1, kDebugResource,
			       "puz id %d (0x%04x) size %d, unknowns %08x, %08x, %08x, %04x",
			       puzid, puzid, ipuzSize, u1, u2, u3, u4);
			for (uint i = 0; i < 5; i++) {
				uint16 strlen = _file->readUint16LE();
				Common::String str;
				for (uint16 j = 0; j < strlen; j++)
					str += _file->readByte();
				debugC(1, kDebugResource, " IPUZ string%d: \"%s\"", i, str.c_str());
			}
			uint16 u5 = _file->readUint16LE();
			uint16 u6 = 0;
			if (_isYoda)
				u6 = _file->readUint16LE();
			debugC(1, kDebugResource, " IPUZ unknowns %04x, %04x", u5, u6);
		}
	}
	break;
	case MKTAG('T', 'I', 'L', 'E'): {
		uint32 size = _file->readUint32LE();
		_tileCount = size / 1028;
		assert(_tileCount * 1028 == size);
		debugC(1, kDebugResource, "Found %s tag, size %d, %d tiles", tag2str(tag),
		       size, _tileCount);
		_tileDataOffset = _file->pos();
		for (uint32 i = 0; i < _tileCount; i++) {
			uint16 unknown1 = _file->readUint16LE();
			uint16 unknown2 = _file->readUint16LE();
			debugC(1, kDebugResource, "Tile #%d (%d, %d)", i, unknown1, unknown2);
			_file->seek(32 * 32, SEEK_CUR);
		}
	}
	break;
	case MKTAG('Z', 'O', 'N', 'E'): {
		if (!_isYoda) {
			uint32 size = _file->readUint32LE();
			debugC(1, kDebugResource, "size: %d", size);
		}
		_zoneCount = _file->readUint16LE();
		debugC(1, kDebugResource, "ZONE tag: %d zones", _zoneCount);
		for (uint16 i = 0; i < _zoneCount; i++) {
			debugCN(1, kDebugResource, "\n");

			// zone header
			uint16 planet = 0;
			if (_isYoda) {
				planet = _file->readUint16LE();
				uint32 size = _file->readUint32LE();
				uint16 zone_id = _file->readUint16LE();
				assert(zone_id == i);
				debugC(1, kDebugResource,
				       "zone entry #%04x (%d): unknowns %04x, size %d", zone_id,
				       zone_id, planet, size);
			}

			uint32 lastTag = this->readTag();
			assert(lastTag == MKTAG('I', 'Z', 'O', 'N'));
		}
	}
	break;
	case MKTAG('I', 'Z', 'O', 'N'): {
		_file->seek(sizeof(uint32), SEEK_CUR); // skip size;

		uint16 width = _file->readUint16LE();
		uint16 height = _file->readUint16LE();
		uint32 zoneType = _file->readUint32LE();
		uint16 padding = 0;
		uint16 planetAgain = 0;

		assert(height == 9 || height == 18);
		assert(width == 9 || width == 18);

		if (_isYoda) {
			padding = _file->readUint16LE(); // always 0xFFFF
			planetAgain = _file->readUint16LE();
		}
		debugC(1, kDebugResource, " %dx%d entries, unknowns %08x, %04x, %04x",
		       width, height, zoneType, padding, planetAgain);

		ZONE z;
		z.width = width;
		z.height = height;
		z.tiles[0] = new uint16[width * height];
		z.tiles[1] = new uint16[width * height];
		z.tiles[2] = new uint16[width * height];

		// tiles
		for (uint16 j = 0; j < height; j++) {
			for (uint16 k = 0; k < width; k++) {
				uint16 u1 = _file->readUint16LE();
				uint16 u2 = _file->readUint16LE();
				uint16 u3 = _file->readUint16LE();
				// debugC(1, kDebugResource, "(tile: %04x, %04x, %04x)", u1, u2,
				// u3);
				z.tiles[0][(j * width) + k] = u1;
				z.tiles[1][(j * width) + k] = u2;
				z.tiles[2][(j * width) + k] = u3;
			}
			debugCN(1, kDebugResource, "\n");
		}
		_zones.push_back(z);

		if (!_isYoda)
			break;

		uint16 hotspotCount = _file->readUint16LE();
		debugC(1, kDebugResource, "zone hospot count %d", hotspotCount);
		for (uint16 j = 0; j < hotspotCount; j++) {
			HOTSPOT *h = this->readHotspot();
			debugC(1, kDebugResource, " zone hotspot data: type %08x, "
			       "unknowns %04x, %04x, %04x, unknown data "
			       "%04x",
			       h->type, h->x, h->y, h->arg1, h->arg2);
			delete h;
		}

		// read auxiliary data
		uint32 lastTag;
		lastTag = this->readTag();
		assert(lastTag == MKTAG('I', 'Z', 'A', 'X'));

		lastTag = this->readTag();
		assert(lastTag == MKTAG('I', 'Z', 'X', '2'));

		lastTag = this->readTag();
		assert(lastTag == MKTAG('I', 'Z', 'X', '3'));

		lastTag = this->readTag();
		assert(lastTag == MKTAG('I', 'Z', 'X', '4'));

		// read actions
		uint16 iactCount = _file->readUint16LE();
		debugC(1, kDebugResource, " IACT count: %d", iactCount);
		for (uint16 j = 0; j < iactCount; j++) {
			lastTag = this->readTag();
			assert(lastTag == MKTAG('I', 'A', 'C', 'T'));
		}
	}
	break;
	default:
		debugC(1, kDebugResource, "Unknown tag %s", tag2str(tag));
		assert(false);
		break;
	}
	return tag;
}

SCRIPT *Resource::readScript() {
	SCRIPT *s = new SCRIPT;
	s->text = NULL;
	s->opcode = _file->readUint16LE();
	for (int i = 0; i < 5; i++)
		s->args[i] = _file->readUint16LE();

	uint16 length = _file->readUint16LE();
	if (length) {
		s->text = (char *)malloc(length);
		_file->read(s->text, length);
	}

	return s;
}

HOTSPOT *Resource::readHotspot() {
	HOTSPOT *h = new HOTSPOT;
	h->type = _file->readUint32LE();
	h->arg1 = _file->readUint16LE();
	h->arg2 = _file->readUint16LE();
	h->x = _file->readUint16LE();
	h->y = _file->readUint16LE();
	return h;
}

ZONE *Resource::getZone(uint num) {
	if (num >= _zones.size()) {
		warning("Resource::getZone(%d) ref is out of range", num);
		return 0;
	}

	return &_zones[num];
}

byte *Resource::getStupData(void) {
	byte *data = new byte[32 * 32 * 9 * 9];
	_file->seek(_stupOffset, SEEK_SET);
	_file->read(data, 32 * 32 * 9 * 9);
	return data;
}

byte *Resource::getTileData(uint32 ref) {
	if (ref >= _tileCount) {
		warning("Resource::getTileData(%d) ref is out of range", ref);
		return 0;
	}

	byte *data = new byte[32 * 32];
	_file->seek(_tileDataOffset + (ref * ((32 * 32) + 4)) + 4, SEEK_SET);
	_file->read(data, 32 * 32);
	return data;
}

uint16 Resource::getTileFlags(uint32 ref, bool upperField) {
	if (ref >= _tileCount) {
		warning("Resource::getTileFlags(%d) ref is out of range", ref);
		return 0;
	}

	_file->seek(_tileDataOffset + (ref * ((32 * 32) + 4)) + (upperField ? 2 : 0),
	            SEEK_SET);
	return _file->readUint16LE();
}

const char *Resource::getTileName(uint32 ref) {
	if (ref >= _tileCount) {
		warning("Resource::getTileName(%d) ref is out of range", ref);
		return 0;
	}

	for (uint32 i = 0; i < _tileNames.size(); i++) {
		if (ref == _tileNames[i].id)
			return _tileNames[i].name.c_str();
	}
	// Tile name not found.
	return 0;
}

const char *Resource::getSoundFilename(uint16 ref) {
	if (ref >= _soundFiles.size()) {
		warning("Resource::getSoundFilename(%d) ref is out of range", ref);
		return 0;
	}

	return _soundFiles[ref].c_str();
}

} // End of namespace Deskadv
