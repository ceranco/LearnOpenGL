#pragma once

namespace shader_sources {
	constexpr auto vertex = R"glsl(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		
		void main()
		{
			gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
	)glsl";

	constexpr auto fragment = R"glsl(
		#version 330 core
		out vec4 fragColor;
		
		void main()
		{
			fragColor = vec4(0.3, 0.5, 0.75, 1.0);
		}
	)glsl";
}