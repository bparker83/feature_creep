#include <conio.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <Windows.h>

#define KEY_TILDE 96
#define KEY_ONE 49
#define KEY_TWO 50
#define KEY_THREE 51
#define KEY_FOUR 52
#define KEY_FIVE 53
#define KEY_SIX 54
#define KEY_SEVEN 55
#define KEY_EIGHT 56
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

struct ColorToggle {
	bool ForegroundBlue;
	bool ForegroundGreen;
	bool ForegroundRed;
	bool ForegroundIntensity;
	bool BackgroundBlue;
	bool BackgroundGreen;
	bool BackgroundRed;
	bool BackgroundIntensity;

	short GetColor() {
		short color = 0;
		if (ForegroundBlue) color += 1;
		if (ForegroundGreen) color += 2;
		if (ForegroundRed) color += 4;
		if (ForegroundIntensity) color += 8;
		if (BackgroundBlue) color += 16;
		if (BackgroundGreen) color += 32;
		if (BackgroundRed) color += 64;
		if (BackgroundIntensity) color += 128;
		return color;
	}
};
class Entity {
public:
	Entity() {}
	~Entity() {}
};

class Food : public Entity {
public:
	Food(char* name, char icon, unsigned short color, COORD position, unsigned short magnitude) :
		name_(name),
		icon_(icon),
		color_(color),
		position_(position),
		magnitude_(magnitude) {}
	~Food() {}
	void EatFood(unsigned short &stamina) const { stamina += magnitude_; return; }
	void ShowFood() {
		HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE); // probably expensive. consider storing as pointer
														  // or create superclass Entity with GetPosition(), GetColor(), etc and let render function do processing
		SetConsoleCursorPosition(std_out, position_);
		SetConsoleTextAttribute(std_out, color_);
		cout << icon_;
	}
	const short GetX() const { return position_.X; }
	const short GetY() const { return position_.Y; }
private:
	unsigned short color_;
	char icon_;
	COORD position_;
	unsigned short magnitude_;
	char* name_;
};

int main() {
	const unsigned short MAP_WIDTH = 30;
	const unsigned short MAP_HEIGHT = 15;
	const unsigned short MAX_FOOD = 20;
	COORD player_pos;
	HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);
	char key_press;
	ColorToggle color{ 1, 1, 1, 1, 0, 0, 0, 0 };
	bool debug = false;
	unsigned short stamina = 10;
	vector<Food*> foods;

	srand(time(NULL));

	for (int i = 0; i < MAX_FOOD; i++) {
		// Food doesn't need position. one Food with several instances on the map
		foods.push_back(new Food("Jerky", 'j', 12, { rand() % MAP_WIDTH + 1, rand() % MAP_HEIGHT + 1 }, 5));
		// needs to validate if Food or Player already exists at location
	}

	player_pos.X = 7;
	player_pos.Y = 7;

	while (true) {
		system("cls");

		if (debug) {
			SetConsoleCursorPosition(std_out, { 0, 0 });
			SetConsoleTextAttribute(std_out, 15);

			cout << "Color: " << color.GetColor() << endl;
			cout << "X: " << player_pos.X << endl;
			cout << "Y: " << player_pos.Y << endl;
			cout << "Stamina: " << stamina << endl;
		}

		for (Food *entity : foods) {
			// ShowFood() needs to be ShowEntity() and in the Entity class. Player can't be considered an Entity until then
			entity->ShowFood();
		}

		SetConsoleCursorPosition(std_out, player_pos);
		SetConsoleTextAttribute(std_out, color.GetColor());
		cout << "@";

		key_press = _getch();

		if (key_press == VK_ESCAPE) break; // quit
		switch (key_press) {
		case KEY_UP:
			if (stamina) {
				player_pos.Y--;
				stamina--;
			}
			break;
		case KEY_DOWN:
			if (stamina) {
				player_pos.Y++;
				stamina--;
			}
			break;
		case KEY_LEFT:
			if (stamina) {
				player_pos.X--;
				stamina--;
			}
			break;
		case KEY_RIGHT:
			if (stamina) {
				player_pos.X++;
				stamina--;
			}
			break;
		case KEY_ONE:
			color.ForegroundBlue = !color.ForegroundBlue;
			break;
		case KEY_TWO:
			color.ForegroundGreen = !color.ForegroundGreen;
			break;
		case KEY_THREE:
			color.ForegroundRed = !color.ForegroundRed;
			break;
		case KEY_FOUR:
			color.ForegroundIntensity = !color.ForegroundIntensity;
			break;
		case KEY_FIVE:
			color.BackgroundBlue = !color.BackgroundBlue;
			break;
		case KEY_SIX:
			color.BackgroundGreen = !color.BackgroundGreen;
			break;
		case KEY_SEVEN:
			color.BackgroundRed = !color.BackgroundRed;
			break;
		case KEY_EIGHT:
			color.BackgroundIntensity = !color.BackgroundIntensity;
			break;
		case KEY_TILDE:
			debug = !debug;
			break;
		}

		for (auto it = foods.begin(); it != foods.end(); ) {
			if (player_pos.X == (*it)->GetX() && player_pos.Y == (*it)->GetY()) {
				(*it)->EatFood(stamina);
				// convert to shared_ptr
				it = foods.erase(it);
			}
			else {
				it++;
			}
		}

		for (Food *entity : foods) {
			// ShowFood() needs to be ShowEntity() and in the Entity class. Player can't be considered an Entity until then
			entity->ShowFood();
		}

		SetConsoleCursorPosition(std_out, player_pos);
		SetConsoleTextAttribute(std_out, color.GetColor());
		cout << "@";
	}

	return 0;
}
