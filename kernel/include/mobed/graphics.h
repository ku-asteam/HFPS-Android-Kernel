#ifndef __MOBED_GRAPHICS__
#define __MOBED_GRAPHICS__

#include <linux/seq_file.h>

enum gfx_state {
	GFX_IN,
	GFX_OUT
};

#define SURFACEFLINGER_COMM "surfaceflinger"
#define MDSS_FB0_COMM "mdss_fb0"
#define RENDERTHREAD_COMM "RenderThread"

extern void mobed_gfx_enable(void);
extern void mobed_gfx_disable(void);
extern bool is_mobed_gfx_enabled(void);

extern void mobed_gfx_init(void);

extern void mobed_surfaceflinger_update(int cpu, enum gfx_state state);
extern void mobed_mdss_fb0_update(int cpu, enum gfx_state state);
extern void mobed_renderthread_update(int cpu, enum gfx_state state);

extern void mobed_graphics_proc_print(struct seq_file *m, void *v);

#endif
