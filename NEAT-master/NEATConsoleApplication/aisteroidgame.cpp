
#include "aisteroidgame.h"

const int Game::NUM_INPUTS = 51;

Game::Game(){
	MAX_FITNESS = 600000000;
}
int Game::clear()
{
	manager.clear();
	return 0;
}
void Game::subset(double *input, int v_x,int v_y, int n)
{
	int x0,y0;
	for (int k = 0;k < n;k++)
	{
		y0 = (v_y - int(n / 2.0) + k);
		if (y0 < 0) { y0 += GRID_Y; }
		else 
		{
			y0 = y0%GRID_Y;
		}
		for (int i = 0;i < n;i++)
		{
			x0= (v_x - int(n / 2.0)+i);
			if (x0 < 0) { x0 += GRID_X; }
			else
			{
				x0 = x0%GRID_X;
			}
			input[k*n+i] = gamma*grid.grid_value[y0][x0]+(1.0-gamma)*oldgrid[k][i];
			oldgrid[k][i] = input[k*n + i];

		}
	}
}

int netOut(Network *net)
{
	int i = 0;
	int action = 0;
	double output = (*(net->outputs.begin()))->activation;
	vector<NNode *>::iterator curNode;

	for (curNode = net->outputs.begin(); curNode != net->outputs.end(); ++curNode) {
		if ((*curNode)->activation > output) {
			output = (*curNode)->activation;
			action = i;
		}
		i++;
	}
	return action;
}
double Game::evalNet(Network *net) {
	int steps = 0;
	double input[NUM_INPUTS];
	int output;
	reset();

	while (steps++ < MAX_FITNESS) {
		subset(input, v_x, v_y, 7);
		if (drawwin)
		{
			for (int i = 0;i < NUM_INPUTS - 2;i++)
			{
				if (i % 7 == 6)
				{
					cout << ((int)(input[i]*100.0))/100.0;
					cout << " " << endl;
				}
				else
				{
					cout << ((int)(input[i] * 100.0)) / 100.0 << " ";
					cout << " ";
				}
			}
		}
		input[49] = .5;
		if (drawwin){
			cout << "   " << endl;
			cout << "  " << endl;
		}
		net->loadSensors(input);

		//Activate the net
		//If it loops, exit returning only fitness of 1 step
		if (!(net->activate())) {
			cout << "LOOPS!" << endl;
			return 1.0;
		}

		output = netOut(net);
		move(output);
		if (gameover) { // if failure stop it now
			cout <<"Score --------------" << score << endl;
			scoreset[min(curset, scoresize - 1)] = score;
			curset++;
			return score;
		}
	}
	return score;
}
int Game::reset()
{
	score = 0;
	sec = 0;
	scoreset[min(curset, scoresize - 1)] = 0;
	gameover = false;
	
	v_x = int(GRID_X / 2);
	v_y = int(GRID_Y / 2);

	manager.clear();
	for (int k = 0;k < GRID_Y;k++)
		for (int i = 0;i < GRID_Y;i++)
			oldgrid[k][i] = 0;
	for (int k = 0;k < n_ast;k++)
	{
		manager.generate({ astpos[k][0],astpos[k][1] }, { astspeed[k][0],astspeed[k][1]});
	}

	return 0;
}
void Game::init() {
	static int firstTime = 1;

	sec = 0;
	score = 0;
	fps = 1;
	v_x = int(GRID_X / 2);
	v_y = int(GRID_Y / 2);
	gameover = false;
	drawwin = false;

	window.create(sf::VideoMode(WINDOW_X, WINDOW_Y), "Test genetaration");

	window.setFramerateLimit(0);

	textureMgr.add("asteroid", PATH_TEXTURES + "asteroids.png");

	float px, py, vx, vy;
	int x, y;
	manager.clear();
	for (int k = 0;k < n_ast;k++)
	{
		px = float(rand() % WINDOW_X);
		py = float(rand() % WINDOW_Y);
		vx = float(rand() % 20 - 10);
		vy = float(rand() % 20 - 10);
		while (abs(px - v_x / GRID_X*WINDOW_X) <= 2 || abs(py - v_y / GRID_Y*WINDOW_Y) <= 2)
		{
			px = float(rand() % WINDOW_X);
			py = float(rand() % WINDOW_Y);
		}
		astpos[k][0] = px;
		astpos[k][1] = py;
		astspeed[k][0] = vx;
		astspeed[k][1] = vy;
		manager.generate({ px,py }, { vx,vy });
	}
}
	

bool Game::isDead() {
	if(grid.grid_value[v_y][v_x]==1) return 1;
	return 0;
}

int Game::move(int action)
{
	
	//init();
	
	if (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			//action = 9;
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::A)
				{
					double avg=0;
					for (int i = max(0, curset - 150);i < curset-1;i++)
					{
						avg += scoreset[i];
					}
					cout << avg / min(150, curset) << endl;
					if (curset == 30000)
					{
						for (int i = curset-150;i < curset - 1;i++)
						{
							cout << scoreset[i] << endl;
						}

					}
				}
				if (event.key.code == sf::Keyboard::R)
				{
					//reset(manager);
				}
				if (event.key.code == sf::Keyboard::Add)
				{
					fps++;
				}
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					fps--;
					if (fps < 1)fps = 1;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					drawwin = !drawwin;
				}
				
				/*
				if (event.key.code == sf::Keyboard::Left)
				{
					action = 0;
					cout << "Left" << endl;
					//v_x -= 1;
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					action = 4;
					cout << "Right" << endl;
					//v_x += 1;
				}
				if (event.key.code == sf::Keyboard::Up)
				{
					action = 2;
					cout << "Up" << endl;
					//v_y -= 1;
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					action = 6;
					cout << "Down" << endl;
					//v_y += 1;
				}
				*/
			}
			
		}
		switch (int(floor(action/2)))
		{
		case 0:
			v_x -= action % 2 + 1;
			break;
		case 1:
			v_y -= action % 2 + 1;
			break;
		case 2:
			v_x += action % 2 + 1;
			break;
		case 3:
			v_y += action % 2 + 1;
			break;
		default:
			break;
		}
		if (v_x >= GRID_X)	v_x = 0;
		if (v_y >= GRID_Y)	v_y = 0;
		if (v_x < 0)	v_x = GRID_X - 1;
		if (v_y < 0)	v_y = GRID_Y - 1;
		//sec = clock.getElapsedTime().asSeconds();
		manager.update(1.53);
		//clock.restart();
		window.clear();
		window.draw(manager);
		if (ENABLE_GRID_TEST_MOD)
		{
			grid.reset();
			for (const auto& it : manager.getAsteroids())
			{
				grid.update(it);
			}
			grid.updateVisual();

		}

		/*
		for (int k = 0;k < 20;k++)
		{
			grid.grid_value[k];
		}*/
		if (isDead())
		{
			gameover = true;
			//cout << "-----------------------You dead-----"<<score<<"----------------" << endl;
			//cout << char(7) << endl;
			//score = 0;
		}
		else
		{
			score++;
		}
		if (drawwin)
		{
			grid.grid_value[v_y][v_x] = 2;
			window.setFramerateLimit(fps);
			grid.updateVisual();
			window.draw(grid);
			window.display();
		}
		else
		{
			window.setFramerateLimit(0);
		}

	}
	
	return 0;

}