#include "inc/bc_log.h"
#include "inc/bc_window.h"
#include "inc/bc_render.h"
#include "inc/bc_camera.h"
#include "inc/bc_shader.h"
#include "inc/bc_texture.h"
#include "inc/bc_clut.h"
#include "inc/bc_transform.h"
#include "inc/bc_cmd.h"
#include "inc/bc_input.h"
#include "inc/bc_key.h"
#include "inc/bc_time.h"

enum bc_clut_e
{
	clut_brick,
	clut_water,
	clut_grass,
	clut_ice,
	clut_yellow_tank,
	clut_green_tank,
	clut_gray_tank,
	clut_red_tank,

	clut_count,
};

int main(int argc, const char** argv)
{
	bc_window window("BattleCity", { 1200, 800 });
	bc_camera camera(256, 224, -10, 1000);
	bc_shader shader("assets/shaders/default.vrtx", "assets/shaders/default.frag");
	bc_render render(2000);
	bc_clut   clut(8, 4);
	
	render.set_shader(&shader);
	render.set_camera(&camera);
	
	clut.set_color(clut_brick, 0, glm::vec4());
	clut.set_color(clut_brick, 1, glm::vec4(203, 77, 12, 255));
	clut.set_color(clut_brick, 2, glm::vec4(166, 0, 0, 255));
	clut.set_color(clut_brick, 3, glm::vec4(117, 117, 117, 255));

	clut.set_color(clut_water, 0, glm::vec4());
	clut.set_color(clut_water, 1, glm::vec4(32, 56, 239, 255));
	clut.set_color(clut_water, 2, glm::vec4(158, 255, 243, 255));
	clut.set_color(clut_water, 3, glm::vec4(32, 56, 239, 255));

	clut.set_color(clut_grass, 0, glm::vec4());
	clut.set_color(clut_grass, 1, glm::vec4(130, 211, 16, 255));
	clut.set_color(clut_grass, 2, glm::vec4(0, 69, 0, 255));
	clut.set_color(clut_grass, 3, glm::vec4(0, 60, 20, 255));

	clut.set_color(clut_ice, 0, glm::vec4());
	clut.set_color(clut_ice, 1, glm::vec4(117, 117, 117, 255));
	clut.set_color(clut_ice, 2, glm::vec4(190, 190, 190, 255));
	clut.set_color(clut_ice, 3, glm::vec4(255, 255, 255, 255));

	clut.set_color(clut_yellow_tank, 0, glm::vec4());
	clut.set_color(clut_yellow_tank, 1, glm::vec4(138, 113, 0, 255));
	clut.set_color(clut_yellow_tank, 2, glm::vec4(255, 154, 56, 255));
	clut.set_color(clut_yellow_tank, 3, glm::vec4(255, 231, 162, 255));

	clut.set_color(clut_green_tank, 0, glm::vec4());
	clut.set_color(clut_green_tank, 1, glm::vec4(0, 81, 0, 255));
	clut.set_color(clut_green_tank, 2, glm::vec4(0, 146, 56, 255));
	clut.set_color(clut_green_tank, 3, glm::vec4(178, 255, 207, 255));

	clut.set_color(clut_gray_tank, 0, glm::vec4());
	clut.set_color(clut_gray_tank, 1, glm::vec4(24, 60, 93, 255));
	clut.set_color(clut_gray_tank, 2, glm::vec4(190, 190, 190, 255));
	clut.set_color(clut_gray_tank, 3, glm::vec4(255, 255, 255, 255));

	clut.set_color(clut_red_tank, 0, glm::vec4(0.f, 0.f, 0.f, 0.f));
	clut.set_color(clut_red_tank, 1, glm::vec4(142.f, 0.f, 117.f, 255.f));
	clut.set_color(clut_red_tank, 2, glm::vec4(219.f, 40.f, 0.f, 255.f));
	clut.set_color(clut_red_tank, 3, glm::vec4(255., 255.f, 255.f, 255.f));

	bc_transform tran;
	bc_texture   brick_texture("assets/textures/bc_brick_tile.png");
	bc_time      time;

	while (!window.is_closing())
	{
		time.update();

		bc_window::poll_events();
		
		render.clear(50, 50, 50, 255);

		glm::vec3 pos(0.f);
		for (i32 y = 0; y < 28; ++y)
		{
			for (i32 x = 0; x < 32; ++x)
			{
				tran.translate(pos);
				render.draw_sprite(&tran, { 0, 0 }, { 8, 8 }, &brick_texture, clut_brick);
				
				pos.x += 8;
			}

			pos.x  = 0;
			pos.y += 8;
		}


		clut.bind(&shader);
		render.submit();

		window.swap_buffers();
	}

	return 0;
}