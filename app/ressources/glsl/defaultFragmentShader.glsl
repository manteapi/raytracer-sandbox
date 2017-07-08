uniform sampler2D texSampler;
varying vec4 fColor;
varying vec2 ftexCoord;

void main()
{
    gl_FragColor = fColor*texture2D(texSampler, ftexCoord);
}
