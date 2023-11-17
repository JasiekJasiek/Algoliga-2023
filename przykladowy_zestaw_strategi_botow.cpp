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

struct Board{
    vector< string > map;
    vector< Player > players;
    Board(const int NumberOfEnemies){
        map.resize(HEIGHT);
        players.resize(NumberOfEnemies + 1);
    }
};

struct BaseStrategy {

    const Point ZEROZERO = {0, 0};
    const Point DEAFULT = {-1, -1};

    const vector< Point > Directions = {
        {-1, 0}, // UP
        {0, 1},  // RIGHT
        {1, 0},  // LEFT
        {0, -1}  // LEFT
    };

    const vector< Point > DiagonalDirections = {
        {-1, -1}, //UP-LEFT
        {-1, 1},  //UP-RIGHT
        {1, 1},   //LEFT-RIGHT
        {1, -1}   //LEFT-LEFT
    };

    bool isOnMap(const Point& point){
        return point.x >= 0 && point.x < HEIGHT && point.y >= 0 && point.y < WIDTH;
    }

};

struct GoToSelectedPointsStrategy : BaseStrategy{ 
    int  where_we_go = 0;

    Point move(const Board& board){
        return goToSelectedPoints(board.players[ MY_ID ].position);
    }

    Point goToSelectedPoints(const Point& playerPosition){
        vector <Point> list_of_points = {{1,1},{3,3},{7,4},{14,12},{8,15},{1,19},{1,1},{12,1},{16,1},{12,6}};
        if(where_we_go >= list_of_points.size()){
            return ZEROZERO;
        }
        if (!(list_of_points[where_we_go] == playerPosition)){
            return list_of_points[ where_we_go ];
        }
        else {
            where_we_go++;
            return list_of_points[ where_we_go ];
        }
        return ZEROZERO;
    }
};

//write program which is going to any of free cell
//TASK
struct StrategyOfGoingAnywhere : BaseStrategy{
    
    Point move(const Board board){
        return goAnywhere(board.map);
    }

    Point goAnywhere(const vector< string >& map){
        for(int i = 0; i < HEIGHT; ++i){
            for(int j = 0; j < WIDTH; ++j){
                if(map[ i ][ j ] == '.'){
                    return {i, j};
                }
            }
        }
        return ZEROZERO;
    }
};

struct StrategyOfGoingToTheNearestFreeCell : BaseStrategy{
    
    Point move(const Board& board){
        return goToTheNearestFreeCell(board.players[ MY_ID ].position, board.map);
    } 

    Point findNearestFreeCellToMe(const Point& playerPosition, const vector< string >& map){
        vector< vector< bool > > vis(HEIGHT, vector< bool >(WIDTH, false));
        queue< Point > q;
        q.push(playerPosition);
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
        return ZEROZERO;
    }

    Point goToTheNearestFreeCell(const Point& playerPosition, const vector< string >& map){
        return findNearestFreeCellToMe(playerPosition, map);
    }
};

struct StrategyOfMakingSquare : StrategyOfGoingToTheNearestFreeCell{
    Point end = DEAFULT;
    Point start = DEAFULT;

    Point move (const Board& board){
        if(playerInsideHisTerytory(board.players[ MY_ID ].position, board.map)){
            cerr << 1 << '\n';
            return goToNearestTerytoryEdge(board.players[ MY_ID ].position, board.map);
        }
        if(noStartedSquare()){
            start = {board.players[ MY_ID ].position.x, board.players[ MY_ID ].position.y};        
            end = lookForSquare(board.players[ MY_ID ].position, board.map);
            if(noGoodSquare()){
                start = DEAFULT;
                return findNearestFreeCellToMe(board.players[ MY_ID ].position, board.map);
            }
        }
        return makeSquare(board.players[ MY_ID ].position, board.map);
    }

    bool playerInsideHisTerytory(const Point& playerPosition, const vector< string >& map){
        return !amIOnEdge(playerPosition, map) && noStartedSquare();
    }

    //check if your bot is on the any edge of your field 
    //TASK
    bool amIOnEdge(const Point& playerPosition, const vector< string >& map){
        for(auto [dx, dy] : Directions){
            Point new_point = {playerPosition.x + dx, playerPosition.y + dy};
            if(isOnMap(new_point)){
                if(map[ new_point.x ][ new_point.y ] == '.'){
                    return true;
                }
            }
        }
        return false;
    }

    Point goToNearestTerytoryEdge(const Point playerPosition, const vector< string >& map){
        int minimal_count = 1 << 20;
        Point target_cell = ZEROZERO;
        for(auto[ dx, dy ] : Directions){
            Point new_position = {playerPosition.x + dx, playerPosition.y + dy};
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
        return target_cell;
    }

    //no stareted square at the moment
    bool noStartedSquare(){
        return end == (Point)DEAFULT && start == (Point)DEAFULT;
    }

    //it's looking for place to make next square 
    Point lookForSquare(const Point& playerPosition, const vector< string >& map){
        const int SIDE_OF_THE_SQUARE = 8;
        const int NUMBER_OF_REPETITIONS_ALLOWED = 5;
        for(auto[ dx, dy ] : DiagonalDirections){
            Point new_point = {playerPosition.x + (dx * SIDE_OF_THE_SQUARE), playerPosition.y + (dy * SIDE_OF_THE_SQUARE)};
            if(isOnMap(new_point)){
                cerr << new_point.x << " " << new_point.y << endl;
                if(map[ new_point.x ][ new_point.y ] == '.' && howManyTimesWillIGoThroughMyCells(new_point, map) <= NUMBER_OF_REPETITIONS_ALLOWED && !isOpponentInside(new_point, map)){
                    return new_point;
                }
            }
        }
        return DEAFULT;
    }

    //you will get two points reprezenting two corner of square and map, write program which checks how many times your bot will go throught already taken cells
    //TASK
    int howManyTimesWillIGoThroughMyCells(Point potencialEndSquare, const vector< string >& map){
        Point startCopy = start;
        if(potencialEndSquare.x > startCopy.x){
            swap(potencialEndSquare.x, startCopy.x);
        }
        if(potencialEndSquare.y > startCopy.y){
            swap(potencialEndSquare.y, startCopy.y);
        }
        int count = 0;
        for(int i = potencialEndSquare.x; i <= startCopy.x; ++i){
            if(map[ i ][ potencialEndSquare.y ] == '0'){
                count++;
            }
        }
        for(int i = potencialEndSquare.x; i <= startCopy.x; ++i){
            if(map[ i ][ startCopy.y ] == '0'){
                count++;
            }
        }
        for(int i = potencialEndSquare.y + 1; i <= startCopy.y; ++i){
            if(map[ potencialEndSquare.x ][ i ] == '0'){
                count++;
            }
        }
        for(int i = potencialEndSquare.y + 1; i <= startCopy.y; ++i){
            if(map[ startCopy.x ][ i ] == '0'){
                count++;
            }
        }
        return count;
    }

    //you will get two points reprezenting two corner of square and map, write program which checks if inside of the square is any cells taken by opponent 
    //TASK
    bool isOpponentInside(Point potencialEndSquare, const vector< string >& map){
        
        Point startCopy = start;
        if(potencialEndSquare.x > startCopy.x){
            swap(potencialEndSquare.x, startCopy.x);
        }
        if(potencialEndSquare.y > startCopy.y){
            swap(potencialEndSquare.y, startCopy.y);
        }
        for(int i = potencialEndSquare.x; i <= startCopy.x; ++i){
            for(int j = potencialEndSquare.y; j <= startCopy.y; ++j){
                if(map[ i ][ j ] == '1' || map[ i ][ j ] == '2' || map[ i ][ j ] == '3'){
                    return true;
                }
            }
        }
        return false;
    }  

    bool noGoodSquare(){
        return end == (Point)DEAFULT;
    }

    Point makeSquare(const Point playerPosition, const vector< string >& map){
        if(hasNotYetComeToEnd(playerPosition)){
            return end;
        }
        else if(comeToEndAtThisMoment(playerPosition)){
            end = DEAFULT;
            return start;
        }
        else if(hasNotYetComeToStart(playerPosition)){
            return start;
        }
        else if(comeToStartAtThisMoment(playerPosition)){
            start = DEAFULT;
            return goToTheNearestFreeCell(playerPosition, map);
        }
        else{
            return goToTheNearestFreeCell(playerPosition, map);
        }
    }

    bool hasNotYetComeToEnd(const Point playerPosition){
        return !(playerPosition == end) && !(end == (Point)DEAFULT);
    }

    bool comeToEndAtThisMoment(const Point playerPosition){
        return playerPosition == end;
    }

    bool hasNotYetComeToStart(const Point playerPosition){
        return !(playerPosition == start);
    }

    bool comeToStartAtThisMoment(const Point playerPosition){
        return playerPosition == start;
    }

};

int main(){

    int NumberOfEnemies;
    int CurrRound;
    cin >> NumberOfEnemies;
    
    Board board(NumberOfEnemies);

    StrategyOfMakingSquare strategy;
    
    while (1) {
        cin >> CurrRound;
        int x, y, _;
        cin >> x >> y >> _;
        int nx = y;
        int ny = x;
        board.players[ MY_ID ].position = {nx, ny};
        for (int i = 0; i < NumberOfEnemies; ++i) {
            cin >> x >> y >> _;
            int nx = y;
            int ny = x;
            board.players[ i + 1 ].position = {nx, ny};
        }
        for (int i = 0; i < HEIGHT; i++) {
            cin >> board.map[ i ];
        }
        
        Point target = strategy.move(board);

        cout << target.y << " " << target.x << '\n';

    }
}
