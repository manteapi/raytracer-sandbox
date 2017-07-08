attribute vec4 vertices;
attribute vec4 colors;
varying vec4 fColor;
attribute vec2 texCoord;
varying vec2 ftexCoord;

void main()
{
    gl_Position = vertices;
    fColor = colors;
    ftexCoord = texCoord;
}
