#include<iostream>
#include<vector>
#include<climits>
#include<iomanip>

#define W1 2
#define W2 3
#define W3 5
#define W4 7
#define W5 4

#define DEPTH 7

using namespace std;

int playingMode = 0;

class Mancala{
private:
    vector<int> board;
    int turn;
    int additionalMoveofPlayer1;
    int additionalMoveofPlayer2;
    int capturedStonesofPlayer1;
    int capturedStonesofPlayer2;

public:
Mancala(int initialStones = 4){
    for(int i=0; i<14; i++){
        if(i==6 || i==13)
            board.push_back(0);
        else
            board.push_back(initialStones);
    }
    //this->turn = turn;
    this->turn = 1;
    additionalMoveofPlayer1 = 0;
    additionalMoveofPlayer2 = 0;
    capturedStonesofPlayer1 = 0;
    capturedStonesofPlayer2 = 0;
}

Mancala(Mancala *game){
    board = game->getBoard();
    turn = game->getTurn();
    additionalMoveofPlayer1 = game->getAdditionalMoveofPlayer1();
    additionalMoveofPlayer2 = game->getAdditionalMoveofPlayer2();
    capturedStonesofPlayer1 = game->getCapturedStonesofPlayer1();
    capturedStonesofPlayer2 = game->getCapturedStonesofPlayer2();
}

vector<int> getBoard(){
    return board;
}

int getTurn(){
    return turn;
}

int getAdditionalMoveofPlayer1(){
    return additionalMoveofPlayer1;
}

int getAdditionalMoveofPlayer2(){
    return additionalMoveofPlayer2;
}

int getCapturedStonesofPlayer1(){
    return capturedStonesofPlayer1;
}

int getCapturedStonesofPlayer2(){
    return capturedStonesofPlayer2;
}

void printBoard(){
    cout << "Player 2" << endl;
    for(int i=12; i>6; i--)
        cout << board[i] << "\t";
    cout << endl;
    cout << "Bin: "<<board[13] <<endl<<endl;
    cout << "Player 1" << endl;
    for(int i=0; i<6; i++)
        cout << board[i] << "\t";
    cout << endl;
    cout<<"Bin: "<< board[6] << endl<<endl;
}

void printBoardUI(){
    cout<<endl;
    cout << "                   [Player #2]" << endl;
    cout<<"\t";
    for (int i = 1; i <= 6; i++) {
        cout << "["  << i << " ] ";
    }
    cout << endl<<"\t";
    for (int i = 12; i>6; i--) {
        cout << setw(2) << board[i] << "   ";
    }
    cout << endl;
    cout << setw(2) << board[13] << "    "; 
    cout << setw(2) << "\t\t\t\t\t"<<board[6] << "   ";
    cout << endl <<"\t";
    for (int i = 0; i < 6; i++) {
        cout << setw(2) << board[1] << "   ";
    }
    cout << endl<<"\t";
    for (int i = 1; i <=6; i++) {
        cout << "[" << setw(2) << i << "] ";
    }
    cout << endl;
    cout << "                  [Player #1]" << setw(22) << endl;
   
    cout << endl;
}

int getScore(int player){
    if(player == 1)
        return board[6];
    else
        return board[13];
}

bool isGameOver(){
    bool player1Empty = true;
    bool player2Empty = true;

    for(int i=0; i<6; i++){
        if(board[i] != 0)
            player1Empty = false;
    }

    if(player1Empty){
        for(int i=7; i<13; i++){
            board[13] += board[i];
            board[i] = 0;
        }
        return true;
    }

    for(int i=7; i<13; i++){
        if(board[i] != 0)
            player2Empty = false;
    }

    if(player2Empty){
        for(int i=0; i<6; i++){
            board[6] += board[i];
            board[i] = 0;
        }
        return true;
    }
    return false;
}

vector<int> getMoves(){
    vector<int> moves;
    if(turn == 1){
        for(int i=0; i<6; i++){
            if(board[i] != 0)
                moves.push_back(i);
        }
    }
    else{
        for(int i=7; i<13; i++){
            if(board[i] != 0)
                moves.push_back(i);
        }
    }
    return moves;
}

void makeMove(int pit){
    int stones = board[pit];
    board[pit] = 0;

    for(int i=stones; i>0; i--){
        pit = (pit+1)%14;
        if((turn == 1 && pit == 13) || (turn == 2 && pit == 6)){
            i++;
            continue;
        }

        if(i == 1 && board[pit] == 0){
            if(turn == 1){
                if(pit >= 0 && pit <= 5){
                capturedStonesofPlayer1 += board[12-pit];
                board[6] += board[12-pit]+1;
                board[12-pit] = 0;
                }
                else if(pit>=6 && pit<=12){
                    board[pit] += 1;
                }
            }
            else if(turn == 2){
                if(pit >= 7 && pit <= 12){
                capturedStonesofPlayer2 += board[12-pit];
                board[13] += board[12-pit]+1;
                board[12-pit] = 0;
            }
            else if(pit == 13 ||(pit>=0 && pit<=5)){
                board[pit] += 1;
            }
            }
        }
        else{
            board[pit] += 1;
        }
    }

    if(turn == 1 && pit == 6){
        additionalMoveofPlayer1 += 1;
    }
    else if(turn == 2 && pit == 13){
        additionalMoveofPlayer2 += 1;
    }
    else{
        turn = 3-turn;
    }
}


int heuristic1(Mancala *game){
    int value = 0;
    vector<int> currentBoard = game->getBoard();
    if(game->getTurn() == 1){
        value = currentBoard[6] -currentBoard[13];
    }
    else{
        value = currentBoard[13] - currentBoard[6];
    }
    return value;
}

int heuristic2(Mancala *game){
    int player1Stones = 0;
    int player2Stones = 0;
    int value = 0;
    vector<int> currentBoard = game->getBoard();
    for(int i=0; i<6; i++){
        player1Stones += currentBoard[i];
    }
    for(int i=7; i<13; i++){
        player2Stones += currentBoard[i];
    }

    if(game->getTurn() == 1){
        value = W1*(currentBoard[6]-currentBoard[13])+W2*(player1Stones-player2Stones);
    }
    else{
        value = W1*(currentBoard[13]-currentBoard[6])+W2*(player2Stones-player1Stones);
    }
    return value;
}

int heuristic3(Mancala *game){
    int value = 0;
    int additionalMove = 0;
    if(game->getTurn() == 1){
        additionalMove = game->getAdditionalMoveofPlayer1();
        value = heuristic2(game)+W3*additionalMove;
    }
    else{
        additionalMove = game->getAdditionalMoveofPlayer2();
        value = heuristic2(game)+W3*additionalMove;
    }
    return value;
}

int heuristic4(Mancala *game){
    int value = 0;
    if(game->getTurn() == 1){
        value = heuristic3(game)+W4*game->getCapturedStonesofPlayer1();
    }
    else{
        value = heuristic3(game)+W4*game->getCapturedStonesofPlayer2();
    }
    return value;
}

int heuristic5(Mancala *game){
    int value = 0;
    int furthestValidBinPlayer1 = -1;
    int furthestValidBinPlayer2 = -1;
    for (int i = 5; i >= 0; --i) {
        if (game->getBoard()[i] > 0) {
            furthestValidBinPlayer1 = i;
            break;
        }
    }
    for (int i = 12; i >= 7; --i) {
        if (game->getBoard()[i] > 0) {
            furthestValidBinPlayer2 = i;
            break;
        }
    }
    if(game->getTurn() == 1){
        value = heuristic1(game)+W5*furthestValidBinPlayer1;
    }
    else{
        value = heuristic1(game)+W5*furthestValidBinPlayer2;
    }

}

int getValue(){
    int heuristic = 0;
    int value = 0;
    if(playingMode == 2){
        if(turn == 1){
            heuristic = 2;
        }
        else{
            heuristic = 3;
        }
    }
    else{
        heuristic = 1;
    }

    switch (heuristic){
        case 1:
            value = heuristic1(new Mancala(this));
            break;
        case 2:
            value = heuristic2(new Mancala(this));
            break;
        case 3:
            value = heuristic3(new Mancala(this));
            break;
        case 4:
            value = heuristic4(new Mancala(this));
            break;
        case 5:
            value = heuristic5(new Mancala(this));
            break;
        default:
            value = heuristic1(new Mancala(this));
            break;
    }
    return value;
}

int minimax(int depth, int alpha, int beta, bool maxPlayer){
    if( depth == 0 || isGameOver()){
        return getValue();
    }

    if(maxPlayer){
        int maxEval = INT_MIN;
        vector<int> moves = getMoves();
        for(int i=0; i<moves.size(); i++){
            Mancala *nextGame = new Mancala(this);
            int prevTurn = turn;
            nextGame->makeMove(moves[i]);
            int eval= 0;
            if(nextGame->getTurn() == prevTurn){
                eval = nextGame->minimax(depth, alpha, beta, true);
            }
            else{
                eval = nextGame->minimax(depth-1, alpha, beta, false);
            }
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if(beta <= alpha){
                break;
            }
        }
        return maxEval;
    }
    else{
        int minEval = INT_MAX;
        vector<int> moves = getMoves();
        for(int i=0; i<moves.size(); i++){
            Mancala *nextGame = new Mancala(this);
            int prevTurn = turn;
            nextGame->makeMove(moves[i]);
            int eval= 0;
            if(nextGame->getTurn() == prevTurn){
                eval = nextGame->minimax(depth, alpha, beta, false);
            }
            else{
                eval = nextGame->minimax(depth-1, alpha, beta, true);
            }
            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if(beta <= alpha){
                break;
            }
        }
        return minEval;
    }
}

int getBestMove(int depth){
    int maxEval = INT_MIN;
    int bestMove = -1;
    vector<int> moves = getMoves();
    for(int i=0; i<moves.size(); i++){
        Mancala *nextGame = new Mancala(this);
        nextGame->makeMove(moves[i]);
        int eval = nextGame->minimax(depth, INT_MIN, INT_MAX, true);
        if(eval > maxEval){
            maxEval = eval;
            bestMove = moves[i];
        }
    }
    return bestMove;
}
};

void playerVSai(Mancala *game, int turn){
    int pit = -1;
while(!game->isGameOver()){
    cout<<endl;
    if(game->getTurn()==turn){
        cout<<"Player "<<turn<<"'s turn"<<endl;
        cout<<"Enter move:(1-6)"<<endl;
        cin>>pit;
        if(game->getTurn()==2){
            pit = 13 - pit;
            if(pit >= 7 && pit <= 12 && game->getBoard()[pit]>0){
                game->makeMove(pit);
            }
            else{
                cout<<"Invalid Move.Retry"<<endl;
            }
        }
        else{
        pit -= 1;
        if(pit>=0 && pit<=5 && game->getBoard()[pit]>0){
            game->makeMove(pit);
        }
        else{
            cout<<"Invalid Move.Retry"<<endl;
        }
        }
    }
    else{
        cout<<"AI's turn"<<endl;
        game->makeMove(game->getBestMove(DEPTH));
    }
     game->printBoard();
    cout<<endl<<endl<<endl;
}
}

void aiVSai(Mancala *game, int turn){
    while(!game->isGameOver()){
        cout<<endl;
        cout<<"Player "<<game->getTurn()<<"'s turn"<<endl;
        game->makeMove(game->getBestMove(DEPTH));
        game->printBoard();
        cout<<endl<<endl<<endl;
    }
}


int main(){
    int n;
    int turn = 1;
    int score1 = 0;
    int score2 = 0;

    cout<<"Enter Mode:"<<endl;
    cout<<"1. Human VS AI"<<endl;
    cout<<"2. AI VS AI"<<endl;
    cin>>n;
    if(n==1 || n==2 ){
        playingMode = n;
    }
    else{
        cout<<"Invalid Mode"<<endl;
        return 0;
    }
    if(playingMode == 1){
        cout<<"Enter player choice:"<<endl;
        cout<<"1. Player 1"<<endl;
        cout<<"2. Player 2"<<endl;
        cin>>n;
        if(n==1 || n==2){
            turn = n;
        }
        else{
            cout<<"Invalid Choice"<<endl;
            return 0;
        }
    }

    else if(playingMode == 2){
        cout<<"Enter AI choice:"<<endl;
        cout<<"1. Player1"<<endl;
        cout<<"2. Player2"<<endl;
        cin>>n;
        if(n==1 || n==2){
            turn = n;
        }
        else{
            cout<<"Invalid Choice"<<endl;
            return 0;
        }
    }

    else{
        cout<<"Invalid Mode"<<endl;
        return 0;
    }

    Mancala *game = new Mancala();
    game->printBoard();
    cout<<endl<<endl<<endl;

   if(playingMode == 1){
         playerVSai(game, turn);
   }
   else {
         aiVSai(game, turn);
   }

   cout<<"Game Over"<<endl;
   score1 = game->getScore(1);
   score2 = game->getScore(2);

   cout<<"Player 1 Score: "<<score1<<endl;
   cout<<"Player 2 Score: "<<score2<<endl;

   if(score1 > score2){
       cout<<"Player 1 Wins"<<endl;
   }
   else if(score2 > score1){
       cout<<"Player 2 Wins"<<endl;
   }
   else{
       cout<<"Draw"<<endl;
   }

   return 0;
}