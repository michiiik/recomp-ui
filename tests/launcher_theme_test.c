#include <assert.h>
#include <math.h>

#include "launcher_theme.h"

static float linear_channel(float c) {
    return c <= 0.04045f ? c / 12.92f : powf((c + 0.055f) / 1.055f, 2.4f);
}

static float luminance(LngColor c) {
    return 0.2126f * linear_channel(c.r) +
           0.7152f * linear_channel(c.g) +
           0.0722f * linear_channel(c.b);
}

static float contrast(LngColor a, LngColor b) {
    float la = luminance(a);
    float lb = luminance(b);
    float hi = la > lb ? la : lb;
    float lo = la > lb ? lb : la;
    return (hi + 0.05f) / (lo + 0.05f);
}

static int same_color(LngColor a, LngColor b) {
    return fabsf(a.r - b.r) < 0.0001f &&
           fabsf(a.g - b.g) < 0.0001f &&
           fabsf(a.b - b.b) < 0.0001f &&
           fabsf(a.a - b.a) < 0.0001f;
}

int main(void) {
    LauncherTheme default_theme = launcher_theme_default();
    LauncherTheme null_theme = launcher_theme_by_name(NULL);
    LauncherTheme unknown_theme = launcher_theme_by_name("unknown");
    LauncherTheme n64_theme = launcher_theme_by_name("n64");
    LauncherTheme stadium2_theme = launcher_theme_by_name("stadium2");

    // Default and N64 routing stay unchanged; unknown names still fall back.
    assert(same_color(default_theme.background, null_theme.background));
    assert(same_color(default_theme.accent, unknown_theme.accent));
    assert(n64_theme.scanlines == 0);
    assert(same_color(n64_theme.accent,
                     lng_rgba(0.878f, 0.227f, 0.184f, 1.0f)));

    // Stadium 2 uses pale blue and cream/gold surfaces, dark navy text, a red
    // primary, and muted-gold secondary accents.
    assert(stadium2_theme.background.r > 0.60f &&
           stadium2_theme.background.g > 0.65f &&
           stadium2_theme.background.b > 0.70f);
    assert(stadium2_theme.panel.r > 0.85f &&
           stadium2_theme.panel.g > 0.80f &&
           stadium2_theme.panel.b > 0.70f);
    assert(stadium2_theme.accent.r > 0.65f &&
           stadium2_theme.accent.g < 0.30f &&
           stadium2_theme.accent.b < 0.30f);
    assert(stadium2_theme.accent2.r > 0.35f &&
           stadium2_theme.accent2.g > 0.25f &&
           stadium2_theme.accent2.b < 0.20f);
    assert(stadium2_theme.text.r < 0.20f &&
           stadium2_theme.text.g < 0.25f &&
           stadium2_theme.text.b < 0.30f);
    assert(stadium2_theme.accent_text.r > 0.90f &&
           stadium2_theme.accent_text.g > 0.90f &&
           stadium2_theme.accent_text.b > 0.90f);
    assert(contrast(stadium2_theme.accent, stadium2_theme.accent_text) > 4.5f);
    assert(stadium2_theme.scanlines == 0);

    // The selected public identifier is exact; leading-space input is fallback.
    assert(same_color(launcher_theme_by_name(" stadium2").accent,
                     default_theme.accent));
    return 0;
}
