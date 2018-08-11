#pragma once

namespace data
{
	constexpr float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
	};

	constexpr float texcoods[] = {
		0.0f, 0.0f, // lower left corner
		1.0f, 0.0f,	// lower right corner
		0.5f, 1.0f,	// top-center corner
	};
}
