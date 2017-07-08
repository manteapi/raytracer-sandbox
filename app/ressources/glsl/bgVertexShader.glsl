attribute vec2 texCoord;
attribute vec2 vertices;
attribute vec4 colors;
varying vec4 fcolors;
varying vec2 fVertex;
varying vec2 ftexCoord;

void main()
{
    gl_Position = vec4(vertices.xy,0,1);
    fcolors = colors;
    fVertex = vertices;
    ftexCoord = texCoord;
}
