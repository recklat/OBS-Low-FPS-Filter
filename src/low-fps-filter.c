#include <obs-module.h>
#include <util/platform.h>
#include <stdint.h>
#include <stdbool.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("obs-low-fps-filter", "en-US")

struct low_fps_filter {
    obs_source_t *context;
    double target_fps;
    uint64_t interval_ns;
    uint64_t last_passed_ns;
};

static const char *low_fps_name(void *unused)
{
    UNUSED_PARAMETER(unused);
    return obs_module_text("LowFPSFilter.Name");
}

static void low_fps_update(void *data, obs_data_t *settings)
{
    struct low_fps_filter *f = data;
    double fps = obs_data_get_double(settings, "fps");

    if (fps < 1.0)
        fps = 1.0;
    if (fps > 30.0)
        fps = 30.0;

    f->target_fps = fps;
    f->interval_ns = (uint64_t)(1000000000.0 / fps);
    f->last_passed_ns = 0;
}

static void *low_fps_create(obs_data_t *settings, obs_source_t *source)
{
    struct low_fps_filter *f = bzalloc(sizeof(*f));
    f->context = source;
    f->target_fps = 5.0;
    f->interval_ns = 200000000ULL;
    f->last_passed_ns = 0;
    low_fps_update(f, settings);
    return f;
}

static void low_fps_destroy(void *data)
{
    bfree(data);
}

static obs_properties_t *low_fps_properties(void *data)
{
    UNUSED_PARAMETER(data);
    obs_properties_t *props = obs_properties_create();
    obs_properties_add_float_slider(props, "fps",
        obs_module_text("LowFPSFilter.FPS"), 1.0, 30.0, 1.0);
    return props;
}

static void low_fps_defaults(obs_data_t *settings)
{
    obs_data_set_default_double(settings, "fps", 5.0);
}

/*
 * This is an asynchronous video filter. Returning NULL drops an incoming
 * frame; OBS continues displaying the last frame that passed through, which
 * produces the low-frame-rate/stutter effect without changing OBS's global FPS.
 */
static struct obs_source_frame *low_fps_filter_video(void *data,
                                                     struct obs_source_frame *frame)
{
    struct low_fps_filter *f = data;
    if (!frame)
        return NULL;

    const uint64_t ts = frame->timestamp;

    if (f->last_passed_ns == 0 || ts < f->last_passed_ns ||
        ts - f->last_passed_ns >= f->interval_ns) {
        f->last_passed_ns = ts;
        return frame;
    }

    return NULL;
}

static struct obs_source_info low_fps_filter_info = {
    .id = "low_fps_filter",
    .type = OBS_SOURCE_TYPE_FILTER,
    .output_flags = OBS_SOURCE_ASYNC_VIDEO,
    .get_name = low_fps_name,
    .create = low_fps_create,
    .destroy = low_fps_destroy,
    .update = low_fps_update,
    .get_defaults = low_fps_defaults,
    .get_properties = low_fps_properties,
    .filter_video = low_fps_filter_video,
};

bool obs_module_load(void)
{
    obs_register_source(&low_fps_filter_info);
    blog(LOG_INFO, "[obs-low-fps-filter] Loaded. Add 'Low FPS Filter' to a camera source.");
    return true;
}
