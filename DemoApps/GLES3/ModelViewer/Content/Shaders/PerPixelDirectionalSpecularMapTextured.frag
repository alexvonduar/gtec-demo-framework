#version 300 es
// BEWARE: This is a example shader and it has not been optimized for speed.


#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform sampler2D Texture0;
uniform sampler2D TextureSpecular;

// Camera space
uniform vec3 LightDirection;

// Material
uniform vec4 MatAmbient;
uniform float MatShininess;


in vec4 v_Color;
in vec3 v_Eye;
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 o_fragColor;

void main()
{
  // set the specular term to black
  vec4 spec = vec4(0.0);

  // Normalize the input normal
  vec3 n = normalize(v_Normal);

  // Calc the intensity as the dot product the max prevents negative intensity values
  float intensity = max(dot(n, LightDirection), 0.0);

  // if the vertex is lit calc the specular term
  if (intensity > 0.0)
  {
    // Calc the half vector
    vec3 h = normalize(LightDirection + normalize(v_Eye));

    // Calc the specular term into spec
    float intSpec = max(dot(h, n), 0.0);
    vec4 col = texture(TextureSpecular, v_TexCoord);
    spec = MatSpecular * (pow(intSpec, MatShininess * col.w) * vec4(col.xyz, 1.0));
  }

  o_fragColor = texture(Texture0, v_TexCoord) * ((v_Color * intensity) + spec + MatAmbient);
}
