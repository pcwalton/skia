/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "gm.h"
#include "SkCanvas.h"

static void make_bm(SkBitmap* bm) {
    bm->allocN32Pixels(60, 60);
    bm->eraseColor(0);

    SkCanvas canvas(*bm);
    SkPaint paint;

    SkPath path;
    path.moveTo(6, 6);
    path.lineTo(6, 54);
    path.lineTo(30, 54);
    canvas.drawPath(path, paint);

    paint.setStyle(SkPaint::kStroke_Style);
    canvas.drawRect(SkRect::MakeLTRB(0.5f, 0.5f, 59.5f, 59.5f), paint);
}

// This creates a close, but imperfect concatenation of
//      scaling the image up by its dst-rect
//      scaling the image down by the matrix' scale
//  The bug was that for cases like this, we were incorrectly trying to take a
//  fast-path in the bitmapshader, but ended up drawing the last col of pixels
//  twice. The fix resulted in (a) not taking the fast-path, but (b) drawing
//  the image correctly.
//
static void test_bitmaprect(SkCanvas* canvas) {
    SkBitmap bm;
    make_bm(&bm);

    canvas->drawBitmap(bm, 150, 45, NULL);

    SkScalar scale = 0.472560018f;
    canvas->save();
    canvas->scale(scale, scale);
    canvas->drawBitmapRect(bm, SkRect::MakeXYWH(100, 100, 128, 128));
    canvas->restore();

    canvas->scale(-1, 1);
    canvas->drawBitmap(bm, -310, 45, NULL);
}

class BitmapRectTestGM : public skiagm::GM {
public:
    BitmapRectTestGM() {

    }

protected:
    SkString onShortName() override {
        return SkString("bitmaprecttest");
    }

    SkISize onISize() override {
        return SkISize::Make(320, 240);
    }

    void onDraw(SkCanvas* canvas) override {
        test_bitmaprect(canvas);
    }

private:
    typedef skiagm::GM INHERITED;
};

//////////////////////////////////////////////////////////////////////////////

DEF_GM( return new BitmapRectTestGM; )
