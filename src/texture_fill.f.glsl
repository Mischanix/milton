#if HAS_TEXTURE_MULTISAMPLE
    #extension GL_ARB_texture_multisample : enable
    #extension GL_ARB_sample_shading : enable
#endif

uniform float u_alpha;

#if HAS_TEXTURE_MULTISAMPLE
    uniform sampler2DMS u_canvas;
#else
    uniform sampler2D u_canvas;
#endif
uniform vec2 u_screen_size;

void
main()
{
#if HAS_TEXTURE_MULTISAMPLE
    vec4 color = texelFetch(u_canvas, ivec2(gl_FragCoord.xy), gl_SampleID);
#else
    vec2 screen_point = vec2(gl_FragCoord.x, gl_FragCoord.y);
    vec2 coord = screen_point / u_screen_size;
    vec4 color = texture(u_canvas, coord);
#endif
    out_color = color;
    out_color *= u_alpha;
}
