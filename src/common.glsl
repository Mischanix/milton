#if HAS_TEXTURE_MULTISAMPLE
    #extension GL_ARB_sample_shading : enable
    #extension GL_ARB_texture_multisample : enable
#endif

// Per-stroke uniforms
uniform vec4 u_brush_color;

// CanvasView elements:
uniform ivec2 u_pan_vector;
uniform ivec2 u_zoom_center;
uniform vec2  u_screen_size;
uniform int   u_scale;
uniform int   u_radius;

vec2
canvas_to_raster_gl(vec2 cp)
{
    vec2 rp = ( ((vec2(u_pan_vector) + cp) / float(u_scale)) + vec2(u_zoom_center) ) / u_screen_size;
    // rp in [0, W]x[0, H]

    //rp /= u_screen_size;
    /* // rp in [0, 1]x[0, 1] */

    rp *= 2.0;
    rp -= 1.0;
    /* // rp in [-1, 1]x[-1, 1] */

    rp.y *= -1.0;

    return vec2(rp);
}

vec2
raster_to_canvas_gl(vec2 raster_point)
{
    vec2 canvas_point = ((raster_point - vec2(u_zoom_center)) * float(u_scale)) - vec2(u_pan_vector);

    return canvas_point;
}

bool
brush_is_eraser()
{
    bool is_eraser = false;
    // Constant k_eraser_color defined in canvas.cc
    if ( u_brush_color == vec4(23,34,45,56) ) {
        is_eraser = true;
    }
    return is_eraser;
}


vec4
blend(vec4 dst, vec4 src)
{
    vec4 result = src + dst*(1.0f-src.a);

    return result;
}
