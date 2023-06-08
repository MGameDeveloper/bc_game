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
	bc_render render(100000);
	bc_clut   clut(8, 4);

	bc_render::enable_blend(true);
	bc_render::enable_debug_output(false);
	bc_texture::init();
	bc_key::init();

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

	bc_transform tile_tran;
	bc_transform tran;
	bc_texture   brick_texture("assets/textures/bc_brick_tile.png");
	bc_texture   nes_font("assets/fonts/nes.png");
	bc_time      time;
	bc_cmd       cmds;
	bc_input     input(&cmds);

	window.register_input(&input);

	cmds.add_axis("forward",  [](float v) { bc_log::trace("foraward: %.2f", v); });
	cmds.add_axis("sideward", [](float v) { bc_log::trace("sidward: %.2f", v);  });
	cmds.add_action("press",   []() {bc_log::info("press");   });
	cmds.add_action("release", []() {bc_log::info("release"); });
	cmds.add_action("repeat",  []() {bc_log::info("repeat");  });

	input.bind_axis(ekey_Up,    ekeymod_Unknown, "forward",   1.f);
	input.bind_axis(ekey_Down,  ekeymod_Unknown, "forward",  -1.f);
	input.bind_axis(ekey_Right, ekeymod_Unknown, "sideward",  1.f);
	input.bind_axis(ekey_Left,  ekeymod_Unknown, "sideward", -1.f);
	input.bind_action(ekey_A, ekeystate_Press,   ekeymod_Unknown, "press");
	input.bind_action(ekey_B, ekeystate_Release, ekeymod_Unknown, "release");
	input.bind_action(ekey_C, ekeystate_Repeat,  ekeymod_Unknown, "repeat");

	tile_tran.set_size(glm::vec3(3.f));

	tran.translate(glm::vec3(32, 32, 0));
	tran.set_size (glm::vec3(8.f));
	tran.scale    (glm::vec3(4.f));

	while (!window.is_closing())
	{
		time.update();

		bc_window::poll_events();
		input.process();

		render.clear(0, 0, 0, 255);

		glm::vec3 pos(0.f);
		glm::vec2 view_size(camera.get_view_size().x / tile_tran.get_size().x, camera.get_view_size().y / tile_tran.get_size().y);
		
		for (i32 y = 0; y < view_size.y; ++y)
		{
			for (i32 x = 0; x < view_size.x; ++x)
			{
				tile_tran.translate(pos);
		
				bc_color rand_color(rand() % 20, rand() % 20, rand() % 20, 255);
				render.draw_sprite(&tile_tran, bc_uv(0.f, 0.f, 8.f, 8.f), NULL, 0, rand_color);
				
				pos.x += tile_tran.get_size().x;
			}
		
			pos.x  = 0;
			pos.y += tile_tran.get_size().y;
		}

		double r_sn = (sin(time.elapsed_time) * 0.5f + 0.5f) * 255;
		double g_cn = (cos(time.elapsed_time) * 0.5f + 0.5f) * 255;
		double b_tn = (tan(time.elapsed_time) * 5.0f + 2.5f) * 255;
		bc_color interpolate_color(r_sn, g_cn, b_tn, 255);
		bc_color white_color(255, 255, 255, 255);
		bc_color rand_color(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

		// size need to be fixed so we can draw big letters
		render.draw_text("BATTLE\n CITY", 8, &tran, &nes_font, clut_brick, interpolate_color, &brick_texture, bc_uv(0.f, 0.f, 8.f, 8.f));

		clut.bind(&shader);
		render.submit();

		window.swap_buffers();
	}

	return 0;
}