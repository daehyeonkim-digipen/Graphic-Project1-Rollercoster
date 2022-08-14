#include "CurveScene.h"
#include "polygon.h"
#include "OpenGL.h"

void CurveScene::Point::update() {
	if (selected == true) {
		if (OpenGLInput.IsLeftMouseDown() == false) {
			selected = false;
			cScene.select = false;
		}
		else {
			x = OpenGLInput.get_mouse_pos().x;
			y = OpenGLInput.get_mouse_pos().y;
		}
	}
	if (tanselected == true) {
		if (OpenGLInput.IsLeftMouseDown() == false) {
			tanselected = false;
			cScene.tanselect = false;
		}
		else {
			tan.x = OpenGLInput.get_mouse_pos().x - x;
			tan.y = OpenGLInput.get_mouse_pos().y - y;
		}
	}
}

Vec2 CurveScene::Hermite(Point* p1, Point* p2, float t) {
	float x = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * p1->x + (pow(t, 3) - 2 * pow(t, 2) + t) * p1->tan.x +
		(-2 * pow(t, 3) + 3 * pow(t, 2)) * p2->x + (pow(t, 3) - pow(t, 2)) * p2->tan.x;
	float y = (2 * pow(t, 3) - 3 * pow(t, 2) + 1) * p1->y + (pow(t, 3) - 2 * pow(t, 2) + t) * p1->tan.y +
		(-2 * pow(t, 3) + 3 * pow(t, 2)) * p2->y + (pow(t, 3) - pow(t, 2)) * p2->tan.y;
	return Vec2{ x, y };
}

Vec2 CurveScene::Catmull(Point* p1, Point* p2, Point* p3, Point* p4, float t) {
	float x = ((-pow(t, 3) + 2 * pow(t, 2) - t) * p1->x + (3 * pow(t, 3) - 5 * pow(t, 2) + 2) * p2->x +
		(-3 * pow(t, 3) + 4 * pow(t, 2) + t) * p3->x + (pow(t, 3) - pow(t, 2)) * p4->x) / 2;
	float y = ((-pow(t, 3) + 2 * pow(t, 2) - t) * p1->y + (3 * pow(t, 3) - 5 * pow(t, 2) + 2) * p2->y +
		(-3 * pow(t, 3) + 4 * pow(t, 2) + t) * p3->y + (pow(t, 3) - pow(t, 2)) * p4->y) / 2;
	return Vec2{ x, y };
}

void CurveScene::update() {
	switch (curve) {
	case (Curve::Hermite):
		for (int i = 1; i <= hermitecount; ++i) {
			if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false &&
				OpenGLInput.IsLeftMouseDown() == true && select == false && tanselect == false) {
				Vec2 pos = Vec2{ hermitepoints[i]->x, hermitepoints[i]->y };
				float size = length(static_cast<Vec2>(OpenGLInput.get_mouse_pos()) - pos);
				if (size <= 35) {
					hermitepoints[i]->selected = true;
					select = true;
					break;
				}
				if (showvec == true) {
					Vec2 trans = Vec2{ hermitepoints[i]->x, hermitepoints[i]->y } + hermitepoints[i]->tan;
					size = length(static_cast<Vec2>(OpenGLInput.get_mouse_pos()) - trans);
					if (size <= 30) {
						hermitepoints[i]->tanselected = true;
						tanselect = true;
						break;
					}
				}
			}
			hermitepoints[i]->update();
		}
		if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false && select == false && tanselect == false) {
			if (OpenGLInput.IsLeftMouseClicked() == true) {
				Vec2 prevpos = Vec2{ hermitepoints[hermitecount]->x, hermitepoints[hermitecount]->y };
				Vec2 newtan = static_cast<Vec2>(OpenGLInput.get_mouse_pos()) - prevpos;
				newtan = Vec2{ newtan.x * 100 / length(newtan), newtan.y * 100 / length(newtan) };
				hermitecount++;
				hermitepoints[hermitecount] = new Point(OpenGLInput.get_mouse_pos().x, OpenGLInput.get_mouse_pos().y, newtan);
			}
		}
		if (OpenGLInput.IsRightMouseClicked() == true && hermitecount > 0) {
			hermitepoints.erase(hermitecount);
			hermitecount--;
		}
		break;
	case(Curve::Catmull):
		for (int i = 1; i <= catmullcount; ++i) {
			if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false &&
				OpenGLInput.IsLeftMouseDown() == true && select == false) {
				Vec2 pos = Vec2{ catmullpoints[i]->x, catmullpoints[i]->y };
				float size = length(static_cast<Vec2>(OpenGLInput.get_mouse_pos()) - pos);
				if (size <= 35) {
					catmullpoints[i]->selected = true;
					select = true;
					break;
				}
			}
			catmullpoints[i]->update();
		}
		if (ImGui::IsAnyWindowHovered() == false && ImGui::IsAnyWindowFocused() == false && select == false) {
			if (OpenGLInput.IsLeftMouseClicked() == true) {
				catmullcount++;
				catmullpoints[catmullcount] = new Point(OpenGLInput.get_mouse_pos().x, OpenGLInput.get_mouse_pos().y);
			}
		}
		if (OpenGLInput.IsRightMouseClicked() == true && catmullcount > 4) {
			catmullpoints.erase(catmullcount);
			catmullcount--;
		}
		break;
	}
}

void CurveScene::draw() {
	ImGui::Begin("Spline Curves");
	ImGui::Checkbox("Show Vector", &showvec);
	ImGui::Checkbox("Show Point", &showpnt);
	ImGui::Checkbox("Loop", &looping);
	ImGui::SliderInt("Line Count", &linecount, 1, 30);
	if (ImGui::Button("Reset\nValues", ImVec2(100, 40))) {
		showvec = true;
		showpnt = true;
		looping = false;
		linecount = 20;
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete\nPoints", ImVec2(100, 40))) {
		if (curve == Curve::Hermite) {
			Hdots.clear();
			hermitepoints.clear();
			hermitepoints[1] = new Point(300, 400, { 100, 0 });
			hermitepoints[2] = new Point(700, 400, { 0, 100 });
			for (int j = 0; j <= linecount; ++j) {
				float t = static_cast<float>(j) / linecount;
				Vec2 dot = Hermite(hermitepoints[1], hermitepoints[2], t);
				Hdots.push_back(Dot{ dot.x, dot.y });
			}
			hermitecount = 2;
		}
		if (curve == Curve::Catmull) {
			Cdots.clear();
			catmullpoints.clear();
			catmullpoints[1] = new Point(300, 300);
			catmullpoints[2] = new Point(300, 500);
			catmullpoints[3] = new Point(700, 500);
			catmullpoints[4] = new Point(700, 300);
			for (int j = 0; j <= linecount; ++j) {
				float t = static_cast<float>(j) / linecount;
				Vec2 dot = Catmull(catmullpoints[1], catmullpoints[2], catmullpoints[3], catmullpoints[4], t);
				Cdots.push_back(Dot{ dot.x, dot.y });
			}
			catmullcount = 4;
		}
	}
	ImGui::End();
	ImGui::Begin("Curve Types");
	if (ImGui::Button("Hermite", ImVec2(100, 40))) {
		curve = Curve::Hermite;
	}
	ImGui::SameLine();
	if (ImGui::Button("Catmull", ImVec2(100, 40))) {
		curve = Curve::Catmull;
	}
	ImGui::End();
	if (curve == Curve::Hermite) {
		drawHcurve();
	}
	if (curve == Curve::Catmull) {
		drawCcurve();
	}
}

void CurveScene::updatedots() {
	Hdots.clear();
	for (int i = 1; i < hermitecount; ++i) {
		for (int j = 0; j <= linecount; ++j) {
			float t = static_cast<float>(j) / linecount;
			Vec2 dot = Hermite(hermitepoints[i], hermitepoints[i + 1], t);
			Hdots.push_back(Dot{ dot.x, dot.y });
		}
	}
	if (looping == true) {
		for (int j = 0; j <= linecount; ++j) {
			float t = static_cast<float>(j) / linecount;
			Vec2 dot = Hermite(hermitepoints[hermitecount], hermitepoints[1], t);
			Hdots.push_back(Dot{ dot.x, dot.y });
		}
	}
	Cdots.clear();
	for (int i = 1; i < catmullcount - 2; ++i) {
		for (int j = 0; j <= linecount; ++j) {
			float t = static_cast<float>(j) / linecount;
			Vec2 dot = Catmull(catmullpoints[i], catmullpoints[i + 1], catmullpoints[i + 2], catmullpoints[i + 3], t);
			Cdots.push_back(Dot{ dot.x, dot.y });
		}
	}
	if (looping == true) {
		std::vector<Point*> loop = { catmullpoints[catmullcount - 2], catmullpoints[catmullcount - 1], catmullpoints[catmullcount], catmullpoints[1], catmullpoints[2], catmullpoints[3] };
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j <= linecount; ++j) {
				float t = static_cast<float>(j) / linecount;
				Vec2 dot = Catmull(loop[i], loop[i + 1], loop[i + 2], loop[i + 3], t);
				Cdots.push_back(Dot{ dot.x, dot.y });
			}
		}
	}
}

void CurveScene::drawHcurve() {
	push_settings();
	no_outline();
	set_fill_color(255, 255, 255);
	for (int i = 1; i <= hermitecount; ++i) {
		if (showpnt == true) {
			draw_ellipse(hermitepoints[i]->x, hermitepoints[i]->y, 25, 25);
		}
		if (showvec == true) {
			push_settings();
			set_line_color(0, 255, 0);
			Vec2 trans = Vec2{ hermitepoints[i]->x, hermitepoints[i]->y } + hermitepoints[i]->tan;
			draw_line(hermitepoints[i]->x, hermitepoints[i]->y, trans.x, trans.y);
			pop_settings();
			push_settings();
			no_outline();
			set_fill_color(0, 255, 0);
			float angle = HALF_PI - atan(hermitepoints[i]->tan.y / hermitepoints[i]->tan.x);
			if (hermitepoints[i]->tan.x < 0) {
				angle += PI;
			}
			apply_rotate(-angle, 0, 0);
			apply_translate(trans.x, trans.y);
			draw_triangle(-11, -7, 0, 14, 11, -8);
			pop_settings();
		}
	}
	push_settings();
	updatedots();
	int countmin = 1;
	if (looping == true) {
		countmin = 0;
	}
	for (int i = 0; i < hermitecount - countmin; ++i) {
		int t = i * (linecount + 1);
		for (int j = 0; j < linecount; ++j) {
			int temp = t + j;
			int temp1 = t + j + 1;
			set_line_color(255, 255, 255);
			draw_line(Hdots[temp].x, Hdots[temp].y, Hdots[temp1].x, Hdots[temp1].y);
		}
	}
	pop_settings();
	pop_settings();
}

void CurveScene::drawCcurve() {
	push_settings();
	no_outline();
	set_fill_color(255, 0, 0);
	draw_ellipse(catmullpoints[1]->x, catmullpoints[1]->y, 25, 25);
	set_fill_color(255, 255, 255);
	for (int i = 2; i <= catmullcount - 1; ++i) {
		if (showpnt == true) {
			draw_ellipse(catmullpoints[i]->x, catmullpoints[i]->y, 25, 25);
		}
	}
	set_fill_color(0, 0, 255);
	draw_ellipse(catmullpoints[catmullcount]->x, catmullpoints[catmullcount]->y, 25, 25);
	push_settings();
	updatedots();
	int countmin = 3;
	if (looping == true) {
		countmin = 0;
	}
	for (int i = 0; i < catmullcount - countmin; ++i) {
		int t = i * (linecount + 1);
		for (int j = 0; j < linecount; ++j) {
			int temp = t + j;
			int temp1 = t + j + 1;
			set_line_color(255, 255, 255);
			draw_line(Cdots[temp].x, Cdots[temp].y, Cdots[temp1].x, Cdots[temp1].y);
		}
	}
	pop_settings();
	pop_settings();
}