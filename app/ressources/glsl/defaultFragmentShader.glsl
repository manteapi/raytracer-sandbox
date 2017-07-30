uniform sampler2D texSampler;
varying vec2 ftexCoord;
varying vec4 fcolors;

void main()
{
    gl_FragColor = fcolors*texture2D(texSampler, ftexCoord);
}
