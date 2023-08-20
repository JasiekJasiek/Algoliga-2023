// Piotr Sulikowski i Jan Szala
#include <bits/stdc++.h>
using namespace std;
const int WIDTH = 35;
const int HEIGHT = 20;
const int NUMBER_OF_ROUND = 350;
const int MY_ID = 0;
const int NUMBER_OF_DIRECTIONS = 4;


struct Point{
    int x, y;
    bool operator==(const Point& other) const{
        return x == other.x && y == other.y;
    }
};

struct Player{
    Point position;
};

struct BaseStrategy {

    const Point ZEROZERO = {0, 0};

    vector< string > map;
    vector< Player > players;

    vector< Point > Directions = {
        {-1, 0}, // UP
        {0, 1},  // RIGHT
        {1, 0},  // LEFT
        {0, -1}  // LEFT
    };

    vector< Point > DiagonalDirections = {
        {-1, -1}, //UP-LEFT
        {-1, 1},  //UP-RIGHT
        {1, 1},   //LEFT-RIGHT
        {1, -1}   //LEFT-LEFT
    };

    bool isOnMap(const Point point){
        return point.x >= 0 && point.x < HEIGHT && point.y >= 0 && point.y < WIDTH;
    }

    void writeOut(const Point& output){
        cout << output.y << " " << output.x << '\n';
    }

    //taking current position, board map and "visited" vector to mark visited cell
    //returning first find free cell
    Point findNearestFreeCell(Point& position, const vector< string >& map, vector< vector< bool > >& vis){
        queue< Point > q;
        q.push(position);
        while(!q.empty()){
            Point temp = q.front(); q.pop();
            vis[ temp.x ][ temp.y ] = true;
            for(auto[ dx, dy ] : Directions){
                int nx = temp.x + dx;
                int ny = temp.y + dy;
                if(isOnMap({nx, ny})){
                    if(!vis[ nx ][ ny ]){
                        if(map[ nx ][ ny ] == '.'){
                            return {nx, ny};
                        }
                        q.push({nx, ny});
                    }
                }
            }
        }
        return {0, 0};
    }

    //you will get two points reprezenting two corner of square and map, write program which checks if inside of the square is any cells taken by opponent 
    //TASK
    bool isOpponentInside(Point start, Point end, const vector< string >& map){
        //"start" and "end" is position of to opposite corners of square and map is representation of board map
        if(end.x > start.x){
            swap(end.x, start.x);
        }
        if(end.y > start.y){
            swap(end.y, start.y);
        }
        for(int i = end.x; i <= start.x; ++i){
            for(int j = end.y; j <= end.y; ++j){
                if(map[ i ][ j ] == '1' || map[ i ][ j ] == '2' || map[ i ][ j ] == '3'){
                    return true;
                }
            }
        }
        return false;
    }

};


//choose some cells and write program which is goint throught all of them
//TASK
struct GoToSelectedPointsStrategy : BaseStrategy{ 
    int  where_we_go = 0;

    void start(const vector< Player >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        goToSelectedPoints(players[MY_ID]);
    }

    void goToSelectedPoints(Player& player){
        vector <Point> list_of_points = {{1,1},{3,3},{7,4},{14,12},{8,15},{1,19},{1,1},{12,1},{16,1},{12,6}};
        if(where_we_go >= list_of_points.size()){
            writeOut(ZEROZERO);
            return;
        }
        if (!(list_of_points[where_we_go] == player.position)){
            writeOut(list_of_points[ where_we_go ]);
        }
        else {
            where_we_go++;
            writeOut(list_of_points[ where_we_go ]);
        }
        return;
    }
};

//write program which is going to any of free cell
//TASK
struct StrategyOfGoingAnywhere : BaseStrategy{
    
    void start(const vector< Player >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        goAnywhere(map);
    }

    void goAnywhere(const vector< string >& map){
        for(int i = 0; i < HEIGHT; ++i){
            for(int j = 0; j < WIDTH; ++j){
                if(map[ i ][ j ] == '.'){
                    writeOut({i, j});
                    return ;
                }
            }
        }
        writeOut(ZEROZERO);
        return ;
    }
};

struct StrategyOfGoingToTheNearestFreeCell : BaseStrategy{

    void start(const vector< Player >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        goToTheNearestFreeCell(players[MY_ID],map);
    } 
    //looking for first free cell

    void goToTheNearestFreeCell(Player& player, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        Point target_cell = findNearestFreeCell(player.position, map, vis);
        writeOut(target_cell);
    //taking current position, board map and visited vector to mark visited cell
    //returning first find free cell       return;
    }
};

struct StrategyOfMakingSquare : StrategyOfGoingToTheNearestFreeCell{
    Point next_square = {-1, -1};
    Point start_position = {-1, -1};

    void start(const vector< Player >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        makeSquare(players[ MY_ID ], map, next_square, start_position);
    }

    //you will get two points reprezenting two corner of square and map, write program which checks how many times your bot will go throught already taken cells
    //TASK
    int howManyTimesWill_I_GoThroughMyCells(Point start, Point end, const vector< string >& map){
        if(end.x > start.x){
            swap(end.x, start.x);
        }
        if(end.y > start.y){
            swap(end.y, start.y);
        }
        int count = 0;
        for(int i = end.x; i <= start.x; ++i){
            if(map[ i ][ end.y ] == '0'){
                count++;
            }
        }
        for(int i = end.x; i <= start.x; ++i){
            if(map[ i ][ start.y ] == '0'){
                count++;
            }
        }
        for(int i = end.y + 1; i <= start.y; ++i){
            if(map[ end.x ][ i ] == '0'){
                count++;
            }
        }
        for(int i = end.y + 1; i <= start.y; ++i){
            if(map[ start.x ][ i ] == '0'){
                count++;
            }
        }
        return count;
    }

    //it's looking for place to make next square 
    Point lookForSquare(Point& point, const vector< string >& map){
        const int SIDE_OF_THE_SQUARE = 8;
        const int NUMBER_OF_REPETITIONS_ALLOWED = 5;
        for(auto[ dx, dy ] : DiagonalDirections){
            Point new_point = {point.x + (dx * SIDE_OF_THE_SQUARE), point.y + (dy * SIDE_OF_THE_SQUARE)};
            if(isOnMap(new_point)){
                if(map[ new_point.x ][ new_point.y ] == '.' && howManyTimesWill_I_GoThroughMyCells(point, new_point, map) <= NUMBER_OF_REPETITIONS_ALLOWED && !isOpponentInside(point, new_point, map)){
                    return new_point;
                }
            }
        }
        return {-1, -1};
    }

    //check if your bot is on the any edge of your field 
    //TASK
    bool am_I_OnEdge(Point& position, const vector< string >& map){
        for(auto [dx, dy] : Directions){
            Point new_point = {position.x + dx, position.y + dy};
            if(isOnMap(new_point)){
                if(map[ new_point.x ][ new_point.y ] == '.'){
                    return true;
                }
            }
        }
        return false;
    }

    void goToNearestEdge(Point position, const vector< string >& map){
        int minimal_count = 1 << 20;
        Point target_cell = {0, 0};
        for(auto[ dx, dy ] : Directions){
            Point new_position = {position.x + dx, position.y + dy};
            int count = 1;
            while(isOnMap(new_position)){
                if(map[ new_position.x ][ new_position.y ] == '.'){
                    break;
                }
                count++;
                new_position = {new_position.x + dx, new_position.y + dy};
            }
            if(minimal_count > count && isOnMap(new_position)){
                minimal_count = count;
                target_cell = new_position;
            }
        }
        writeOut(target_cell);
    }

    //no stareted square at the moment
    bool no_start_square(Point& start, Point& end){
        return end == (Point){-1, -1} && start == (Point){-1, -1};
    }

    void makeSquare(Player player, const vector< string >& map, Point& start, Point& end){
        //if you are not in the edge of your field and you are not making any square at the moment
        if(!am_I_OnEdge(player.position, map) && no_start_square(start, end)){
            goToNearestEdge(player.position, map);
            return;
        }
        //if you are not making any square at the moment
        if(no_start_square(start, end)){
            start = {player.position.x, player.position.y};
            end = lookForSquare(player.position, map);
            if(end == (Point){-1, -1}){
                start = {-1, -1};
                goToTheNearestFreeCell(player, map);
                return;
            }
            writeOut(end);
            return;
        }
        //if you won't reach end point already
        if(!(player.position == end) && !(end == (Point){-1, -1})){
            writeOut(end);
            return;
        }
        //if you reach end point
        else if(player.position == end){
            end = {-1, -1};
            writeOut(start);
            return;
        }
        //if you won't reach start point already
        else if(!(player.position == start)){
            writeOut(start);
            return;
        }
        //if you reach start point
        else if(player.position == start){
            start = {-1, -1};
            makeSquare(player, map, start, end);
            return;
        }
        else{
            goToTheNearestFreeCell(player, map);
            return;
        }
    }
};

struct StrategyOfMergingBfs : StrategyOfMakingSquare{

    void start(const vector< Player >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategyOfMeargingBfs(players, map);
    }

    vector <vector<int>> addingDistanceFromPlayerToAllCells(const Point& position, const vector< string >& map, vector< vector< bool > >& vis){
        vector< vector< int > >distance(HEIGHT, vector< int >(WIDTH, 0));
        queue< Point > q;
        q.push(position);
        while(!q.empty()){
            Point temp = q.front(); q.pop();
            vis[ temp.x ][ temp.y ] = true;
            for(auto[ dx, dy ] : Directions){
                int nx = temp.x + dx;
                int ny = temp.y + dy;
                if(isOnMap({nx, ny})){
                    if(!vis[ nx ][ ny ]){
                        distance[nx][ny] = distance[temp.x][temp.y]+1; 
                        vis[nx][ny] = true;
                        q.push({nx, ny});
                    }
                }
            }
        }
        return distance;
    }

    vector< vector< int > > makingMapWhoIsCloser ( const vector< Player >& players, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        vector<vector<int>> distance_to_me = addingDistanceFromPlayerToAllCells(players[MY_ID].position, map, vis);
        vector< vector< bool > >vis_opo(HEIGHT, vector< bool >(WIDTH, false));
        vector<vector<int>> distance_to_enemy = addingDistanceFromPlayerToAllCells(players[1].position, map, vis_opo);
        vector< vector< int > >distance_map(HEIGHT, vector< int >(WIDTH, 0));
        for (int i = 0;i < HEIGHT;i++){
            for (int j = 0;j < WIDTH;j++){
                int how_much = distance_to_enemy[i][j] - distance_to_me[i][j];
                distance_map[i][j] = ((how_much <= 0)? -1 : 1) * distance_to_me[i][j];
            }
        }
        return distance_map;
    }

    void dividingPathToParts(const Point n, vector< vector< bool > >& vis,vector<vector<int>>& number_of_sequence){
        vis[n.x][n.y] = false;
        for (Point i : Directions){
            Point u = {n.x+i.x,n.y+i.y};
            if (isOnMap(u) == false){
                continue;
            }
            if (vis[u.x][u.y] == true){
                number_of_sequence[u.x][u.y] = number_of_sequence[n.x][n.y];
                vis[n.x][n.y] = false;
                dividingPathToParts(u,vis,number_of_sequence);
            }
        }
    }

    vector<vector<int>> continuity(int player_number, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        vector<Point> where_path;
        for (int i = 0;i < HEIGHT;i++){
            for (int j = 0;j < WIDTH;j++){
                if (map[i][j] == player_number+'0'){
                    vis[i][j] = true;
                    where_path.push_back({i,j});
                }
            }
        }
        vector<vector<int>> number_of_sequence(HEIGHT, vector< int >(WIDTH, -3));
        int next = 1;
        for (Point g : where_path){
            if (vis[g.x][g.y]){
                number_of_sequence[g.x][g.y] = next;
                dividingPathToParts(g,vis,number_of_sequence);
                next++;
            }
        }
        return number_of_sequence;

    }
    // faze alpha
    Point decisionWhereToGo( vector< Player >& players, const vector< string >& map,vector< vector< int > >& who_is_closer){
        vector<vector<int>> number_of_sequence = continuity(MY_ID,map);
        return {0,0};
    }
    
    void strategyOfMeargingBfs ( vector< Player >& players, const vector< string >& map){
        vector< vector< int > > who_is_closer = makingMapWhoIsCloser (players, map);
        decisionWhereToGo(players, map, who_is_closer);
        writeOut(ZEROZERO);
    }
};

int main(){

    int NumberOfEnemies;
    int CurrRound;
    cin >> NumberOfEnemies;
    vector< Player > players(NumberOfEnemies + 1);
    vector< string > map(HEIGHT);

    StrategyOfMakingSquare soms;
    
    while (1) {
        cin >> CurrRound;
        int x, y, _;
        cin >> x >> y >> _;
        int nx = y;
        int ny = x;
        players[ MY_ID ].position = {nx, ny};
        for (int i = 0; i < NumberOfEnemies; ++i) {
            cin >> x >> y >> _;
            int nx = y;
            int ny = x;
            players[ i + 1 ].position = {nx, ny};
        }
        for (int i = 0; i < HEIGHT; i++) {
            cin >> map[ i ];
        }
        soms.start(players,map);
    }
}
