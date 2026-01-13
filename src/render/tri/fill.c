#include "fill.h"

void swap(void** p1, void** p2) {
	void* temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

void scanline(Pixel* buffer, FillVertex* p0, FillVertex* p1, int width, int height) {
	if (p0->pos.x > p1->pos.x) {
		swap((void*)&p0, (void*)&p1);
	}
	int length = (int)(p1->pos.x - p0->pos.x + 0.5);
	for (int i = 0; i < length; i++) {
		Vec3 pos = vec3Lerp(p0->pos, p1->pos, i / (p1->pos.x - p0->pos.x));
		Color color = colorLerp(p0->color, p1->color, i / (p1->pos.x - p0->pos.x));

		int x = i + (int)(p0->pos.x + 0.5);
		int y = (int)(p0->pos.y + 0.5);

		if (x < 0 || x >= width || y < 0 || y >= height) continue;

		write(&buffer[y * width + x], pixel(color.r / pos.z, color.g / pos.z, color.b / pos.z, color.a / pos.z, pos.z));
	}
}

void trifill(Pixel* buffer, FillVertex p0, FillVertex p1, FillVertex p2, int width, int height) {
	FillVertex* tp0 = &p0;
	FillVertex* tp1 = &p1;
	FillVertex* tp2 = &p2;
	if (tp0->pos.y > tp2->pos.y) {
		swap((void*)&tp0, (void*)&tp2);
	}
	if (tp1->pos.y > tp2->pos.y) {
		swap((void*)&tp1, (void*)&tp2);
	} else if (tp0->pos.y > tp1->pos.y) {
		swap((void*)&tp0, (void*)&tp1);
	}
	float tp01z = 1 / tp0->pos.z;
	float tp11z = 1 / tp1->pos.z;
	float tp21z = 1 / tp2->pos.z;
	for (float i = 0; i < tp1->pos.y-tp0->pos.y; i++) {
		float t = i / (tp1->pos.y - tp0->pos.y);
		float t2 = i / (tp2->pos.y - tp0->pos.y);
		FillVertex v = (FillVertex) {
			.pos = vec3Lerp(tp0->pos, tp1->pos, t),
			.color = colorLerp(colorDiv(tp0->color, tp0->pos.z), colorDiv(tp1->color, tp1->pos.z), t)
		};
		v.pos.z = tp01z + t * (tp11z - tp01z);
		v.pos.y = i + tp0->pos.y;
		FillVertex v2 = (FillVertex) {
			.pos = vec3Lerp(tp0->pos, tp2->pos, t2),
			.color = colorLerp(colorDiv(tp0->color, tp0->pos.z), colorDiv(tp2->color, tp2->pos.z), t2)
		};
		v2.pos.z = tp01z + t2 * (tp21z - tp01z);
		v2.pos.y = i + tp0->pos.y;
		scanline(buffer, &v, &v2, width, height);
	}
	for (float i = 0; i < tp2->pos.y-tp1->pos.y; i++) {
		float t = i / (tp2->pos.y - tp1->pos.y);
		float t2 = (i + tp1->pos.y - tp0->pos.y) / (tp2->pos.y - tp0->pos.y);
		FillVertex v = (FillVertex) {
			.pos = vec3Lerp(tp1->pos, tp2->pos, t),
			.color = colorLerp(colorDiv(tp1->color, tp1->pos.z), colorDiv(tp2->color, tp2->pos.z), t)
		};
		v.pos.z = tp11z + t * (tp21z - tp11z);
		v.pos.y = i + tp1->pos.y;
		FillVertex v2 = (FillVertex) {
			.pos = vec3Lerp(tp0->pos, tp2->pos, t2),
			.color = colorLerp(colorDiv(tp0->color, tp0->pos.z), colorDiv(tp2->color, tp2->pos.z), t2)
		};
		v2.pos.z = tp01z + t2 * (tp21z - tp01z);
		v2.pos.y = i + tp1->pos.y;
		scanline(buffer, &v, &v2, width, height);
	}
}