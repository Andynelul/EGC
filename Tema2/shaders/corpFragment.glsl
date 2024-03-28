#version 330

// Input
//in vec3 frag_color;
//in vec3 frag_normal;

// Uniform properties
uniform float hp;
uniform vec3 color;
//uniform sampler2D texture_1;
//uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) 
out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    out_color=vec4(color+hp/6,1);
   // out_color = vec4(0.5+hp/3,0.9-hp/10,0.5+hp/10,1);

}
