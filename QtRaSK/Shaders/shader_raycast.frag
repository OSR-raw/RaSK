#version 330 core
 
in vec4 pass_Color;
in vec4 glPos;

out vec4 color;
 
uniform sampler2D backface;
uniform sampler3D text3d;
uniform sampler2D transfer;
uniform int do_color;
uniform vec3 cam_view;
//#define Lightning
void main()
{
	float step = 1.0/256.0;
	float a_corr = (512.0*step);
	float search_step = 1.0/96.0;
	
	//if ( length(glPos.xy/glPos.w) < 0.2 )
		//step = 1.0/512.0;
	
	vec2 UV = ( glPos.xy/glPos.w + 1.0)/2.0;
	vec4 frag = texture(backface, UV);
	vec3 start = pass_Color.xyz;
	if ( do_color == 0)
	{
		start = cam_view;
	}	
	
	vec3 dir = (frag.xyz - start);
	
	float len = length(dir);
	if ( len < step )
		discard;
	
	dir = normalize(dir);
	
	vec3 ray_step = dir*step;
	float step_len = length(ray_step);
	
	vec3 s_ray_step = dir*search_step;
	float s_step_len = length(s_ray_step);
	
	float alpha_acc = 0.0;
    float len_acc = 0.0;
	
	float alpha_sample = 0.0f;
	vec3 grad = vec3( 0, 0, 0 );
	vec3 color_acc = vec3(0,0,0);
	vec4 frag3d = texture(text3d, start);
	vec4 Col_sm;
	vec2 UV_T;
	if ( frag3d.a == 0 )
	while ( true )
	{
		start += s_ray_step;
		len_acc += s_step_len;
		
		if ( len_acc >= len )
			break;
					
		frag3d = texture(text3d, start);
		
		
		if ( frag3d.a != 0 )
		{
			{
				vec3 ray_stp = s_ray_step/2;
				float stp_len = s_step_len/2;
				start -= ray_stp;
				len_acc -= stp_len;
				
				frag3d = texture(text3d, start);

				int iter = 0;
				int prev = 1;
				int curr = 1;
				for ( iter; iter< 6; iter++ )
				{
					if(  prev != curr )
					{
						ray_stp /= 2;
						stp_len /=2;				
					}
					prev = curr;
					if ( frag3d.a == 0 )
					{
						start += ray_stp;
						len_acc += stp_len;
						curr = 0;
					}
					else
					{
						start -= ray_stp;
						len_acc -= stp_len;	
						curr = 1;
					}
					frag3d = texture(text3d, start);
				}
				if ( frag3d.a == 0 )
				{
					start += ray_stp;
					len_acc += stp_len;			
				}
				break;
			}			
		}
	}


#ifdef Lightning
	vec3 light = vec3( 0, -50, 50 );
	vec3 lightHalfDir = light/2.0;
#endif
	
	while ( alpha_acc < 1.0 )
	{
		frag3d = texture(text3d, start);
		UV_T.x = frag3d.a;		
		grad = frag3d.rgb * 2.0 - 1.0;
		UV_T.y = 1.0 - length(grad);

		Col_sm = texture(transfer, UV_T);
		
		if ( Col_sm.a != 0 )
		{
			alpha_sample = Col_sm.a*frag3d.a;
			//alpha_sample = 1.0 - (1.0 - pow( alpha_sample, a_corr ));
#ifdef Lightning
			//if ( alpha_sample > 0.9 )
			{
				vec3 normal = -normalize(grad);
				float diff = clamp(dot(normal, light), 0.6, 1.0);
				float sat = pow(clamp(dot(normal, lightHalfDir),0.0,1.0), 20.0);

				float scalar = clamp(length(grad) / (0.5 * 0.5 * 0.5), 0.0, 1.0);
				Col_sm.bgr = Col_sm.bgr * vec3(1.0,1.0,1.0) * clamp(diff + sat, 0.0, 1.0) * scalar;			
			}
#endif		
			color_acc += (1 - alpha_acc)*Col_sm.bgr* alpha_sample ;
			//alpha_acc += (1 - alpha_acc)*alpha_sample;
			alpha_acc += alpha_sample;
		}		
		
		start += ray_step;
		len_acc += step_len;
		
		if ( len_acc >= len )
			break;
	}
	
	//if ( alpha_acc < 0.01 )
	//	discard;
		
	color.rgb = color_acc;//*alpha_acc;
	color.a = alpha_acc;
}