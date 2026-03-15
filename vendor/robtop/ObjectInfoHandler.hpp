#pragma once
#include <cocos2d.h>

class ObjectInfoHandler {

public:
    static char const* getObjectNameFromKey(int key) {
		switch(key) {
			case 1:
				return "Block";
			case 2:
				return "Outline Grid Block";
			case 3:
				return "Corner Outline Grid Block";
			case 4:
				return "Corner Grid Block";
			case 5:
				return "Filling Grid Block";
			case 6:
				return "Corner Pillar Grid Block";
			case 7:
				return "Pillar Grid Block";
			case 8:
				return "Spike";
			case 9:
				return "Ground Spike";
			case 10:
				return "Revert Gravity Portal";
			case 11:
				return "Flip Gravity Portal";
			case 12:
				return "Cube Portal";
			case 13:
				return "Ship Portal";
			case 15:
				return "Big Pulsing Rod";
			case 16:
				return "Medium Pulsing Rod";
			case 17:
				return "Small Pulsing Rod";
			case 18:
				return "Giant Spiky Decoration";
			case 19:
				return "Big Spiky Decoration";
			case 20:
				return "Medium Spiky Decoration";
			case 21:
				return "Small Spiky Decoration";
			case 22:
				return "Default Enter Effect Trigger";
			case 23:
				return "Upwards Enter Effect Trigger";
			case 24:
				return "Downwards Enter Effect Trigger";
			case 25:
				return "Sideways Enter Effect Trigger 1";
			case 26:
				return "Sideways Enter Effect Trigger 2";
			case 27:
				return "Shrinking Enter Effect Trigger";
			case 28:
				return "Growing Enter Effect Trigger";
			case 29:
				return "BG Color Trigger";
			case 30:
				return "Ground Color Trigger";
			case 31:
				return "Start Position";
			case 32:
				return "Enable Icon Effect Trigger";
			case 33:
				return "Disable Icon Effect Trigger";
			case 34:
				return "Level End Trigger";
			case 35:
				return "Yellow Pad";
			case 36:
				return "Yellow Orb";
			case 39:
				return "Flat Spike";
			case 40:
				return "Default Slab";
			case 41:
				return "Big Chain";
			case 42:
				return "BG Effect On Trigger";
			case 43:
				return "BG Effect Off Trigger";
			case 45:
				return "Mirror Portal";
			case 46:
				return "Restore Mirror Portal";
			case 47:
				return "Ball Portal";
			case 48:
				return "Big Fading Cloud Decoration";
			case 49:
				return "Medium Fading Cloud Decoration";
			case 50:
				return "Circle Pulse Decoration";
			case 51:
				return "Hollow Circle Pulse Decoration";
			case 52:
				return "Heart Pulse Decoration";
			case 53:
				return "Diamond Pulse Decoration";
			case 54:
				return "Star Pulse Decoration";
			case 55:
				return "Random Enter Effect Trigger";
			case 56:
				return "Left Random Enter Effect Trigger";
			case 57:
				return "Right Random Enter Effect Trigger";
			case 58:
				return "Imploding Enter Effect Trigger";
			case 59:
				return "Exploding Enter Effect Trigger";
			case 60:
				return "Note Pulse Decoration";
			case 61:
				return "Wavy Ground Spike";
			case 62:
				return "Wavy Slab";
			case 63:
				return "Corner Outline Wavy Slab";
			case 64:
				return "Corner Wavy Slab";
			case 65:
				return "Right Wavy Slab";
			case 66:
				return "Left Wavy Slab";
			case 67:
				return "Blue Pad";
			case 68:
				return "Cornered Wavy Slab";
			case 69:
				return "Mosaic Block";
			case 70:
				return "Outline Mosaic Block";
			case 71:
				return "Corner Outline Mosaic Block";
			case 72:
				return "Corner Mosaic Block";
			case 73:
				return "Filling Mosaic Block";
			case 74:
				return "Corner Pillar Mosaic Block";
			case 75:
				return "Pillar Mosaic Block";
			case 76:
				return "Chiseled Block"; // what the fuck do you call those blocks bruh :sob:
			case 77:
				return "Outline Chiseled Block";
			case 78:
				return "Corner Outline Chiseled Block";
			case 79:
				return "Corner Chiseled Block";
			case 80:
				return "Filling Chiseled Block";
			case 81:
				return "Corner Pillar Chiseled Block";
			case 82:
				return "Pillar Chiseled Block";
			case 83:
				return "Grid Block";
			case 84:
				return "Blue Orb";
			case 85:
				return "Giant Saw Decoration 1";
			case 86:
				return "Big Saw Decoration 1";
			case 87: 
				return "Medium Saw Decoration 1";
			case 88:
				return "Big Saw";
			case 89:
				return "Medium Saw";
			case 90:
				return "Black Block";
			case 91:
				return "Outline Black Block";
			case 92:
				return "Corner Outline Black Block";
			case 93:
				return "Corner Black Block";
			case 94:
				return "Filling Black Block";
			case 95:
				return "Corner Pillar Black Block";
			case 96:
				return "Pillar Black Block";
			case 97:
				return "Small Saw Decoration 1";
			case 98:
				return "Small Saw";
			case 99:
				return "Normal Size Portal";
			case 101:
				return "Small Size Portal";
			case 103:
				return "Medium Spike";
			case 104:
				return "Line Color Trigger";
			case 105:
				return "Obj Color Trigger";
			case 106:
				return "Wide Big Chain";
			case 107:
				return "Wide Medium Chain";
			case 110:
				return "Medium Chain";
			case 111:
				return "Ufo Portal";
			case 112:
				return "fucked up ufo portal";
			case 113:
				return "Big Pixel Cloud Decoration";
			case 114:
				return "Medium Pixel Cloud Decoration";
			case 115:
				return "Small Pixel Cloud Decoration";
			case 116:
				return "Brick Block";
			case 117:
				return "Outline Brick Block";
			case 118:
				return "Corner Outline Brick Block";
			case 119:
				return "Corner Brick Block";
			case 120:
				return "Filling Brick Block";
			case 121:
				return "Corner Pillar Brick Block";
			case 122:
				return "Pillar Brick Block";
			case 123:
				return "Bush 1";
			case 124:
				return "Bush 2";
			case 125:
				return "Bush 3";
			case 126:
				return "Bush 4";
			case 127:
				return "Bush 5";
			case 128:
				return "Bush 6";
			case 129:
				return "Medium Cloud";
			case 130:
				return "Big Cloud";
			case 131:
				return "Small Cloud";
			case 132:
				return "Arrow Pulse Decoration";
			case 133:
				return "Exclamation Pulse Decoration";
			case 134:
				return "Small Diamond Decoration";
			case 135:
				return "Bush Ground Spike";
			case 136:
				return "Interrogation Pulse Decoration";
			case 137:
				return "Big Saw Decoration 2";
			case 138:
				return "Medium Saw Decoration 2";
			case 139:
				return "Small Saw Decoration 2";
			case 140:
				return "Pink Pad";
			case 141:
				return "Pink Orb";
			case 143:
				return "Breakable Block";
			case 144:
				return "Invisible Spike";
			case 145:
				return "Medium Invisible Spike";
			case 146:
				return "Invisible Block";
			case 147:
				return "Invisible Slab";
			case 148:
				return "Square Pulse Decoration";
			case 149:
				return "Triangle Pulse Decoration";
			case 150:
				return "X Pulse Decoration";
			case 151:
				return "Big Spike Decoration";
			case 152:
				return "Medium Spike Decoration";
			case 153:
				return "Small Spike Decoration";
			case 154:
				return "Big Saw Decoration 3";
			case 155:
				return "Medium Saw Decoration 3";
			case 156:
				return "Small Saw Decoration 3";
			case 157:
				return "Ground Decoration";
			case 158:
				return "Left Ground Decoration";
			case 159:
				return "Right Ground Decoration";
			case 160:
				return "Fading Block";
			case 161:
				return "Outline Fading Block";
			case 162:
				return "Corner Outline Fading Block";
			case 163:
				return "Corner Fading Block";
			case 164:
				return "Corner Filling Fading Block";
			case 165:
				return "Corner Pillar Fading Block";
			case 166:
				return "Pillar Fading Block";
			case 167:
				return "Double Corner Fading Block";
			case 168:
				return "Center Fading Block";
			case 169:
				return "Check Block";
			case 170:
			case 192:
				return "Modern Slab";
			case 171:
			case 172:
				return "Right Modern Slab";
			case 173:
				return "Corner Outline Modern Slab";
			case 174:
				return "Extended Corner Modern Slab";
			case 175:
				return "Modern Block";
			case 176:
				return "Tiny Modern Slab";
			case 177:
				return "Modern Spike";
			case 178:
				return "Flat Modern Spike";
			case 179:
				return "Medium Modern Spike";
			case 180:
				return "Big Saw Decoration 4";
			case 181:
				return "Medium Saw Decoration 4";
			case 182:
				return "Small Saw Decoration 4";
			case 183:
			case 186:
				return "Big Modern Saw";
			case 184:
			case 187:
				return "Medium Modern Saw";
			case 185:
			case 188:
				return "Small Modern Saw";
			case 190:
				return "Big Diamond Decoration";
			case 191:
				return "Fake Spike";
			case 193:
				return "Filling Fading Block";
			case 194:
				return "Corner Modern Slab";
			case 195:
				return "Tiny Block";
			case 196:
				return "Tiny Slab";
			case 197:
				return "Short Modern Slab";
			case 198:
				return "Flat Fake Spike";
			case 199:
				return "Medium Fake Spike";
			default:
				return "Unknown";
		}
	}
};