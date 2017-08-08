attribute vec2 texCoord;
attribute vec3 vertices;
attribute vec4 colors;
varying vec4 fcolors;
varying vec2 ftexCoord;

void main()
{
    gl_Position = vec4(vertices.xyz,1);
    fcolors = colors;
    ftexCoord = texCoord;
}
