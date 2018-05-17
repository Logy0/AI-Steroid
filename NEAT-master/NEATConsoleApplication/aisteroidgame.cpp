
#include "aisteroidgame.h"
const int Game::NUM_INPUTS = 51;

Game::Game(){
	MAX_FITNESS = 60000;
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

void Game::init() {
	static int firstTime = 1;
	char curWord[20];
	int id;
	cout << "Level 1" << endl;
	ifstream iFile("level1", ios::in);
	iFile >> curWord;
	iFile >> id;
	level = new Level(id, iFile);
	level->iteAs = 0;
	level->iteIt = 0;
	sec = 0;
	score = 0;
	fps = 25;
	v_x = GRID_X / 2;
	v_y = GRID_Y / 2;
	(mode == 1) ? drawwin = true : drawwin = false;
	gameover = false;
	display_grid = false;

	if (mode == 1)
	{
		window.create(sf::VideoMode(WINDOW_X, WINDOW_Y), "Test genetaration");
		window.setFramerateLimit(0);
	}

	textureMgr.add("asteroid", PATH_TEXTURES + "asteroids.png");
	textureMgr.add("bullet", PATH_TEXTURES + "bullet.png");
	textureMgr.add("vessel", PATH_TEXTURES + "vessel.png");
	vessel.settexture(textureMgr);
	
	float px, py, vx, vy;
	float meanradius = 10;
	//cout <<"AS "<< level->As[0]->x << " " << level->As[0]->y << " " << level->As[0]->vx << " " << level->As[0]->vy << endl;
	manager.clear();
	for (int k = 0;k < n_ast;k++)
	{
		px = float(rand() % WINDOW_X);
		py = float(rand() % WINDOW_Y);
		vx = float(rand() % 200 - 100);
		vy = float(rand() % 200 - 100);
		while (abs(px - float(v_x) / GRID_X*WINDOW_X) <= 70 && abs(py - float(v_y) / GRID_Y*WINDOW_Y) <= 70)
		{
			px = float(rand() % WINDOW_X);
			py = float(rand() % WINDOW_Y);
		}
		manager.generate({ px,py }, { vx, vy }, meanradius);

		astpos[k][0] = px;
		astpos[k][1] = py;
		astspeed[k][0] = vx;
		astspeed[k][1] = vy;
	}
	/*
	astpos[n_ast-1][0] = level->As[0]->x;
	astpos[1][1] = level->As[0]->y;
	astspeed[1][0] = level->As[0]->vx;
	astspeed[1][1] = level->As[0]->vy;
	*/
}
int Game::reset()
{
	score = 0;
	sec = 0;
	level->iteAs = 0;
	level->iteIt = 0;
	scoreset[min(curset, scoresize - 1)] = 0;
	gameover = false;
	float meanradius = 10;
	v_x = int(GRID_X / 2);
	v_y = int(GRID_Y / 2);

	manager.clear();
	
	vessel.killed = false;
	vessel.reset({ 250,250});
	tdir = THRUST_DIRECTION::NONE;
	
	for (int k = 0;k < GRID_Y;k++)
		for (int i = 0;i < GRID_Y;i++)
			oldgrid[k][i] = 0;
	for (int k = 0;k < n_ast;k++)
	{
		manager.generate({ astpos[k][0],astpos[k][1] }, { astspeed[k][0],astspeed[k][1] }, meanradius);
	}

	return 0;
}
int Game::spawnAs()
{
	float x, y,vx,vy;
	bool flag = true;
	
	while (flag && level->iteAs+1<level->As.size())
	{
		if(score == level->As.at(level->iteAs + 1)->fc)
		{ 
			x = level->As.at(level->iteAs + 1)->x;
			y = level->As.at(level->iteAs + 1)->y;
			vx = level->As.at(level->iteAs + 1)->vx;
			vy = level->As.at(level->iteAs + 1)->vy;
			manager.generate({x,y}, {vx,vy}, level->As.at(level->iteAs + 1)->avgcircle);
			level->iteAs++;
		}
		else { flag = false; }
	}
	
	return 0;
}
int Game::spawnIt()
{
	if (score == level->It.at(level->iteIt + 1)->fc)//IMPLEMENT LEVELS; //HUM ... IMPLEMENT ITEMS
	{

	}
	return 0;
}


bool Game::isDead() {
	if(vessel.killed) return 1;
	return 0;
}

int Game::move(int action)
{
	do
	{
		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{

				if (event.key.code == sf::Keyboard::A)
				{
					double avg = 0;
					for (int i = max(0, curset - 150);i < curset - 1;i++)
					{
						avg += scoreset[i];
					}
					cout << avg / min(150, curset) << endl;
					if (curset == 30000)
					{
						for (int i = curset - 150;i < curset - 1;i++)
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
				if (event.key.code == sf::Keyboard::G)
				{
					drawwin = !drawwin;
				}

				if (event.key.code == sf::Keyboard::Z)
					tdir |= THRUST_DIRECTION::UP;
				if (event.key.code == sf::Keyboard::S)
					tdir |= THRUST_DIRECTION::DOWN;
				if (event.key.code == sf::Keyboard::Q)
					tdir |= THRUST_DIRECTION::LEFT;
				if (event.key.code == sf::Keyboard::D)
					tdir |= THRUST_DIRECTION::RIGHT;
				if (event.key.code == sf::Keyboard::Space)
					vessel.fire();
			}
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Z)
					tdir &= ~THRUST_DIRECTION::UP;
				if (event.key.code == sf::Keyboard::S)
					tdir &= ~THRUST_DIRECTION::DOWN;
				if (event.key.code == sf::Keyboard::Q)
					tdir &= ~THRUST_DIRECTION::LEFT;
				if (event.key.code == sf::Keyboard::D)
					tdir &= ~THRUST_DIRECTION::RIGHT;
			}


		}

		switch (action)
		{
		case 0:
			tdir |= THRUST_DIRECTION::UP;
			break;
		case 1:
			tdir |= THRUST_DIRECTION::DOWN;
			break;
		case 2:
			tdir |= THRUST_DIRECTION::LEFT;
			break;
		case 3:
			tdir |= THRUST_DIRECTION::RIGHT;
			break;
		case 4:
			tdir &= ~THRUST_DIRECTION::UP;
			break;
		case 5:
			tdir &= ~THRUST_DIRECTION::DOWN;
			break;
		case 6:
			tdir &= ~THRUST_DIRECTION::LEFT;
			break;
		case 7:
			tdir &= ~THRUST_DIRECTION::RIGHT;
			break;
		case 8:
			//vessel.fire();
			break;
		default:
			tdir &= THRUST_DIRECTION::NONE;
			break;
		}
		/*
		if (v_x >= GRID_X)	v_x = 0;
		if (v_y >= GRID_Y)	v_y = 0;
		if (v_x < 0)	v_x = GRID_X - 1;
		if (v_y < 0)	v_y = GRID_Y - 1;
		*/
		float delta = 0.025;
		manager.update(delta);
		bulletManager.update(delta);

		vessel.thrust(static_cast<THRUST_DIRECTION>(tdir));
		vessel.update(delta);

		//clock.restart();
			
		v_x = (int)(GRID_X * vessel.getPosition().x / WINDOW_X);
		v_y = (int)(GRID_Y * vessel.getPosition().y / WINDOW_Y);
		grid.grid_value[v_y][v_x] = 2;

		collisionsManager.check();

		
			
			
		if (ENABLE_GRID_TEST_MOD)
		{
			grid.reset();
			for (const auto& it : manager.getAsteroids())
			{
				grid.update(it);
			}
			grid.updateVisual();

		}

		
			
		if (drawwin)
		{
			window.clear();
			window.draw(manager);
			window.draw(bulletManager);
			window.draw(vessel);

			window.setFramerateLimit(fps);

			grid.updateVisual();
			if (display_grid)
			{
				window.draw(grid);
			}
			
			window.display();
		}
		else
		{
			window.setFramerateLimit(0);
		}

		if (isDead())
		{
			gameover = true;
			if (mode == 1)
			{
				cout << "-----------------------You dead-----" << score << "----------------" << endl;
				//cout << char(7) << endl;
				reset();
			}

		}
		else
		{
			score++;
			spawnAs();
			//spawnIt();
		}

	
	} while (mode == 1 && window.isOpen());

	return 0;

}