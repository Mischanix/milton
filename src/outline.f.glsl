
in vec2 v_sizes;

uniform int u_radius;
uniform bool u_fill;
uniform vec4 u_color;


void
main()
{
    float r = length(v_sizes);
    float radius = float(u_radius);
    float girth = u_fill ? 2.0 : 1.0;
    const float ring_alpha = 0.4;

    if ( r <= radius && r > radius - girth ) {
        out_color = vec4(0.0,0.0,0.0,ring_alpha);
    }
    else if ( r < radius + girth && r >= radius ) {
        out_color = vec4(1.0,1.0,1.0,ring_alpha);
    }
    else if ( u_fill && r < radius ) {
        out_color = u_color;
    }
    else {
        discard;
    }
}

