uniform sampler2D texSampler;
varying vec2 fVertex;
varying vec2 ftexCoord;
varying vec4 fcolors;
varying vec4 outcolors;

void main()
{
    gl_FragColor = fcolors*texture2D(texSampler, ftexCoord);
}
