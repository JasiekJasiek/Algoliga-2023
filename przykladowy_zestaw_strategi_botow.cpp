// Piotr Sulikowski i Jan Szala
#include <bits/stdc++.h>
using namespace std;
const int WIDTH = 35;
const int HEIGHT = 20;
const int NUMBER_OF_ROUND = 350;
const int MY_ID = 0;
const int NUMBER_OF_DIRECTIONS = 4;


struct POINT{
    int x, y;
    bool operator==(const POINT other) const{
        return x == other.x && y == other.y;
    }
};

struct PLAYER{
    POINT position;
};

struct BASE {

    POINT ZEROZERO = {0, 0};

    vector< string > map;
    vector< PLAYER > players;

    vector< POINT > DIRECTIONS = {
        {-1, 0}, // UP
        {0, 1},  // RIGHT
        {1, 0},  // LEFT
        {0, -1}  // LEFT
    };

    vector< POINT > DIAGONAL_DIRECTIONS = {
        {-1, -1}, //UP-LEFT
        {-1, 1},  //UP-RIGHT
        {1, 1},   //LEFT-RIGHT
        {1, -1}   //LEFT-LEFT
    };

    bool is_on_map(POINT point){
        return point.x >= 0 && point.x < HEIGHT && point.y >= 0 && point.y < WIDTH;
    }

    void write_out(POINT* output){
        cout << output -> y << " " << output -> x << '\n';
    }
};


//choose some cells and write program which is goint throught all of them
//TASK
struct STRATEGY_OF_GOING_TO_SELECTED_POINTS : BASE{ 
    int  where_we_go = 0;

    void start(const vector< PLAYER >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategy_of_going_to_selected_points(players[MY_ID]);
    }

    void strategy_of_going_to_selected_points(PLAYER& player){
        vector <POINT> list_of_points = {{1,1},{3,3},{7,4},{14,12},{8,15},{1,19},{1,1},{12,1},{16,1},{12,6}};
        if(where_we_go >= list_of_points.size()){
            write_out(&ZEROZERO);
            return;
        }
        if (!(list_of_points[where_we_go] == player.position)){
            write_out(&list_of_points[ where_we_go ]);
        }
        else {
            where_we_go++;
            write_out(&list_of_points[ where_we_go ]);
        }
        return;
    }
};

//write program which is going to any of free cell
//TASK
struct STRATEGY_OF_GOING_ANYWHERE : BASE{
    
    void start(const vector< PLAYER >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategy_of_going_anywhere(map);
    }

    void strategy_of_going_anywhere(const vector< string >& map){
        for(int i = 0; i < HEIGHT; ++i){
            for(int j = 0; j < WIDTH; ++j){
                if(map[ i ][ j ] == '.'){
                    POINT target = {i, j};
                    write_out(&target);
                    return ;
                }
            }
        }
        write_out(&ZEROZERO);
        return ;
    }
};

struct STRATEGY_OF_GOING_TO_THE_NEAREST_FREE_CELL : BASE{

    void start(const vector< PLAYER >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategy_of_going_to_the_nearest_free_cell(players[MY_ID],map);
    } 
    //looking for first free cell
    POINT bfs(POINT& position, const vector< string >& map, vector< vector< bool > >& vis){
        queue< POINT > q;
        q.push(position);
        while(!q.empty()){
            POINT temp = q.front(); q.pop();
            vis[ temp.x ][ temp.y ] = true;
            for(auto[ dx, dy ] : DIRECTIONS){
                int nx = temp.x + dx;
                int ny = temp.y + dy;
                if(is_on_map({nx, ny})){
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

    void strategy_of_going_to_the_nearest_free_cell(PLAYER& player, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        POINT target_cell = bfs(player.position, map, vis);
        write_out(&target_cell);
        return;
    }
};

struct STRATEGY_OF_MAKING_SQUARE : STRATEGY_OF_GOING_TO_THE_NEAREST_FREE_CELL{
    POINT next_square = {-1, -1};
    POINT start_position = {-1, -1};

    void start(const vector< PLAYER >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategy_of_making_square(players[ MY_ID ], map, next_square, start_position);
    }

    //you will get two points reprezenting two corner of square and map, write program which checks how many times your bot will go throught already taken cells
    //TASK
    int how_many_times_will_I_go_through_my_cells(POINT start, POINT end, const vector< string >& map){
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


    //you will get two points reprezenting two corner of square and map, write program which checks if inside of the square is any cells taken by opponent 
    //TASK
    bool is_opponent_inside(POINT start, POINT end, const vector< string >& map){
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

    //it's looking for place to make next square 
    POINT look_for_square(POINT& point, const vector< string >& map){
        const int SIDE_OF_THE_SQUARE = 8;
        const int NUMBER_OF_REPETITIONS_ALLOWED = 5;
        for(auto[ dx, dy ] : DIAGONAL_DIRECTIONS){
            POINT new_point = {point.x + (dx * SIDE_OF_THE_SQUARE), point.y + (dy * SIDE_OF_THE_SQUARE)};
            if(is_on_map(new_point)){
                if(map[ new_point.x ][ new_point.y ] == '.' && how_many_times_will_I_go_through_my_cells(point, new_point, map) <= NUMBER_OF_REPETITIONS_ALLOWED && !is_opponent_inside(point, new_point, map)){
                    return new_point;
                }
            }
        }
        return {-1, -1};
    }

    //check if your bot is on the any edge of your field 
    //TASK
    bool am_I_on_edge(POINT& position, const vector< string >& map){
        for(auto [dx, dy] : DIRECTIONS){
            POINT new_point = {position.x + dx, position.y + dy};
            if(is_on_map(new_point)){
                if(map[ new_point.x ][ new_point.y ] == '.'){
                    return true;
                }
            }
        }
        return false;
    }

    void go_to_nearest_edge(POINT position, const vector< string >& map){
        int minimal_count = 1 << 20;
        POINT target_cell = {0, 0};
        for(auto[ dx, dy ] : DIRECTIONS){
            POINT new_position = {position.x + dx, position.y + dy};
            int count = 1;
            while(is_on_map(new_position)){
                if(map[ new_position.x ][ new_position.y ] == '.'){
                    break;
                }
                count++;
                new_position = {new_position.x + dx, new_position.y + dy};
            }
            if(minimal_count > count && is_on_map(new_position)){
                minimal_count = count;
                target_cell = new_position;
            }
        }
        write_out(&target_cell);
    }

    //no stareted square at the moment
    bool no_start_square(POINT& start, POINT& end){
        return end == (POINT){-1, -1} && start == (POINT){-1, -1};
    }

    void strategy_of_making_square(PLAYER player, const vector< string >& map, POINT& start, POINT& end){
        //if you are not in the edge of your field and you are not making any square at the moment
        if(!am_I_on_edge(player.position, map) && no_start_square(start, end)){
            go_to_nearest_edge(player.position, map);
            return;
        }
        //if you are not making any square at the moment
        if(no_start_square(start, end)){
            start = {player.position.x, player.position.y};
            end = look_for_square(player.position, map);
            if(end == (POINT){-1, -1}){
                start = {-1, -1};
                strategy_of_going_to_the_nearest_free_cell(player, map);
                return;
            }
            write_out(&end);
            return;
        }
        //if you won't reach end point already
        if(!(player.position == end) && !(end == (POINT){-1, -1})){
            write_out(&end);
            return;
        }
        //if you reach end point
        else if(player.position == end){
            end = {-1, -1};
            write_out(&start);
            return;
        }
        //if you won't reach start point already
        else if(!(player.position == start)){
            write_out(&start);
            return;
        }
        //if you reach start point
        else if(player.position == start){
            start = {-1, -1};
            strategy_of_making_square(player, map, start, end);
            return;
        }
        else{
            strategy_of_going_to_the_nearest_free_cell(player, map);
            return;
        }
    }
};

struct STRATEGY_OF_MEARGING_BFS : STRATEGY_OF_GOING_TO_THE_NEAREST_FREE_CELL{

    void start(const vector< PLAYER >& players_now, const vector< string >& map_now){
        players = players_now;
        map = map_now;
        strategy_of_mearging_bfs(players, map);
    }

    vector <vector<int>> bfs_2(POINT& position, const vector< string >& map, vector< vector< bool > >& vis){
        vector< vector< int > >distance(HEIGHT, vector< int >(WIDTH, 0));
        queue< POINT > q;
        q.push(position);
        while(!q.empty()){
            POINT temp = q.front(); q.pop();
            vis[ temp.x ][ temp.y ] = true;
            for(auto[ dx, dy ] : DIRECTIONS){
                int nx = temp.x + dx;
                int ny = temp.y + dy;
                if(is_on_map({nx, ny})){
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

    vector< vector< int > > making_map_who_is_closer ( vector< PLAYER >& players, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        vector<vector<int>> distance_to_me = bfs_2(players[MY_ID].position, map, vis);
        vector< vector< bool > >vis_opo(HEIGHT, vector< bool >(WIDTH, false));
        vector<vector<int>> distance_to_enemy = bfs_2(players[1].position, map, vis_opo);
        vector< vector< int > >distance_map(HEIGHT, vector< int >(WIDTH, 0));
        for (int i = 0;i < HEIGHT;i++){
            for (int j = 0;j < WIDTH;j++){
                int how_much = distance_to_enemy[i][j] - distance_to_me[i][j];
                distance_map[i][j] = ((how_much <= 0)? -1 : 1) * distance_to_me[i][j];
            }
        }
        return distance_map;
    }

    void dfs(POINT n, vector< vector< bool > >& vis,vector<vector<int>>& number_of_sequence){
        vis[n.x][n.y] = false;
        for (POINT i : DIRECTIONS){
            POINT u = {n.x+i.x,n.y+i.y};
            if (is_on_map(u) == false){
                continue;
            }
            if (vis[u.x][u.y] == true){
                number_of_sequence[u.x][u.y] = number_of_sequence[n.x][n.y];
                vis[n.x][n.y] = false;
                dfs(u,vis,number_of_sequence);
            }
        }
    }

    vector<vector<int>> continuity(int player_number, const vector< string >& map){
        vector< vector< bool > >vis(HEIGHT, vector< bool >(WIDTH, false));
        vector<POINT> where_path;
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
        for (POINT g : where_path){
            if (vis[g.x][g.y]){
                number_of_sequence[g.x][g.y] = next;
                dfs(g,vis,number_of_sequence);
                next++;
            }
        }
        return number_of_sequence;

    }
    // faze alpha
    POINT decision_where_to_go( vector< PLAYER >& players, const vector< string >& map,vector< vector< int > >& who_is_closer){
        vector<vector<int>> number_of_sequence = continuity(MY_ID,map);
        return {0,0};
    }
    
    void strategy_of_mearging_bfs ( vector< PLAYER >& players, const vector< string >& map){
        vector< vector< int > > who_is_closer = making_map_who_is_closer (players, map);
        decision_where_to_go(players, map, who_is_closer);
        write_out(&ZEROZERO);
    }
};

int main(){

    int NUMBER_OF_ENEMIES;
    int CURR_RUND;
    cin >> NUMBER_OF_ENEMIES;
    vector< PLAYER > players(NUMBER_OF_ENEMIES + 1);
    vector< string > map(HEIGHT);

    STRATEGY_OF_MAKING_SQUARE soms;
    
    while (1) {
        cin >> CURR_RUND;
        int x, y, _;
        cin >> x >> y >> _;
        int nx = y;
        int ny = x;
        players[ MY_ID ].position = {nx, ny};
        for (int i = 0; i < NUMBER_OF_ENEMIES; ++i) {
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
