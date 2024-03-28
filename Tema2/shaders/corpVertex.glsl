#version 330

// Input
layout(location = 0) in vec3 v_position;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float hp;
// Output


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
  //  printf("%d",hp.x);
  if(v_position.y>v_position.x)
    gl_Position = Projection * View * Model * vec4(v_position.x,v_position.y+hp*(hp-3)/6,v_position.z, 1.0);
   else  if(v_position.x>v_position.z-0.2||-v_position.x<v_position.z+0.2)
   {
       gl_Position = Projection * View * Model * vec4(v_position.x+hp*(hp-3)/(hp+1),v_position.y,v_position.z, 1.0);

   }
   else  if(v_position.x<v_position.z+0.2||-v_position.x>v_position.z-0.2)
   {
       gl_Position = Projection * View * Model * vec4(v_position.x,v_position.y,v_position.z+hp*(hp-3)/(hp+1), 1.0);

   }
    else
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
