#include <algorithm>
#include <random>
#include <iostream>
#include <climits>
#include "board.hpp"
#include "engine.hpp"
#include <chrono>


int evaluation(const Board& board){
    std::string board_string = board_to_str(board.data.board_0);
    int n = board_string.size();
    int w_p = 0, w_r = 0, w_b = 0, b_p = 0, b_r = 0, b_b = 0;
    // std::vector<U8> pieces = {board.data.b_rook_ws,board.data.b_rook_bs,board.data.b_bishop,board.data.b_pawn_bs,board.data.b_rook_ws,
    //                         board.data.w_rook_ws,board.data.w_rook_bs,board.data.w_bishop,board.data.w_pawn_bs,board.data.w_rook_ws};

    // for (int i = 0; i < 12; i++)
    // {   
    //     if (pieces[i]==DEAD){
    //             continue;
    //         }
    //     if ((board.data.board_0[pieces[i]] & (BLACK|WHITE)) == WHITE){
    //         if ((board.data.board_0[pieces[i]] & ROOK) == ROOK)
    //         {
    //             w_r+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & BISHOP) == BISHOP)
    //         {
    //             w_b+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & PAWN) == PAWN)
    //         {
    //             w_p+=1;
    //         }
    //     }
    //     else
    //     {
    //         if ((board.data.board_0[pieces[i]] & ROOK) == ROOK)
    //         {
    //             b_r+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & BISHOP) == BISHOP)
    //         {
    //             b_b+=1;
    //         }
    //         else if ((board.data.board_0[pieces[i]] & PAWN) == PAWN)
    //         {
    //             b_p+=1;
    //         }
    //     }
        
    // }
    for (int i=0;i<n;i++){
        switch (board_string[i]) {
            case 'P':
                w_p+=1;
                break;
            case 'R':
                w_r+=1;
                break;
            case 'B':
                w_b+=1;
                break;
            case 'p':
                b_p+=1;
                break;
            case 'r':
                b_r+=1;
                break;
            case 'b':
                b_b+=1;
                break;
            default:
                break;
        }
    }
    int ans;
    ans = 100*(w_r-b_r)+80*(w_b-b_b)+40*(w_p-b_p);
    if (board.data.player_to_play==(PlayerColor)BLACK){
        ans *= -1;
    }
    if (board.in_check()){
        ans-=100;
    }
    auto move_s = board.get_legal_moves();
    std::vector<U16> moves(move_s.begin(),move_s.end());
    ans+=moves.size();
    Board dummyboard = *board.copy();
    dummyboard.do_move(0);
    if (board.in_check()){
        ans+=1000;
    }
    auto move_s_dummy = dummyboard.get_legal_moves();
    std::vector<U16> moves_dummy(move_s_dummy.begin(),move_s_dummy.end());
    ans -= moves_dummy.size();
    if (moves_dummy.size()==0){
        ans -= 1000000;
    }
    // if(board.data.player_to_play==1<<6){
    //     ans = w_p+5*w_r+3*w_b-b_p-5*b_r-3*b_b;
    //     if (board.in_check()){
    //         ans-=10;
    //     }
    //     auto move_s = board.get_legal_moves();
    //     std::vector<U16> moves(move_s.begin(),move_s.end());
    //     ans+=moves.size()/100;
    //     // if (moves.size()==0){
    //     //     ans = -float('inf');
    //     // }
        
        
    // }
    // else{
    //     ans = (w_p+5*w_r+3*w_b-b_p-5*b_r-3*b_b);
    //     if (board.in_check()){
    //         ans-=10;
    //     }
    //     auto move_s = board.get_legal_moves();
    //     std::vector<U16> moves(move_s.begin(),move_s.end());
    //     ans+=moves.size()/100;
    //     // if (moves.size()==0){
    //     //     ans = -float('inf');
    //     // }
    // }
    return ans;
}

int minmax(const Board& board, int depth, int alpha, int beta) 
    {
        auto move_s = board.get_legal_moves();
        std::vector<U16> moves(move_s.begin(),move_s.end());
        if (depth == 0 || moves.size()==0) {
            return evaluation(board);
        }

        if (depth%2 == 1) {
            int maxEval = INT_MIN;
            for (size_t i = 0; i < moves.size(); i++) {
                Board b = *board.copy();  // Create a copy of the board
                b.do_move(moves[i]);
                int ans = minmax(b, depth - 1, alpha, beta);
                maxEval = std::max(ans, maxEval);
                alpha = std::max(alpha, ans);
                if (beta <= alpha) {
                    break;
                }
            }

            return maxEval;
        } else {
            int minEval = INT_MAX;
            for (size_t i = 0; i < moves.size(); i++) {
                Board b = *board.copy();  // Create a copy of the board
                b.do_move(moves[i]);
                int ans = minmax(b, depth - 1, alpha, beta);
                minEval = std::min(ans, minEval);
                beta = std::min(beta, ans);
                if (beta <= alpha) {
                    break;
                }
            }

            return minEval;
        }
    }
void Engine::find_best_move(const Board& b) {

    // pick a random move
    auto start_time = std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    std::vector<U16> moves(moveset.begin(),moveset.end());
    int opt_move = 0;
    // if (b.data.player_to_play==(1<<6)){
        
    int opt_val = INT_MIN;
    for (int i=0;i<moves.size();i++){
        auto b_copy = *b.copy();
        b_copy.do_move(moves[i]);
        int x = minmax(b_copy,3,INT_MIN,INT_MAX);
        if (x>opt_val){
            opt_val=x;
            opt_move=i;
        }
    }
    // }
    // else{
    //     int opt_val = INT_MAX;
    //     for (int i=0;i<moves.size();i++){
    //         auto b_copy = *b.copy();
    //         b_copy.do_move(moves[i]);
    //         int x = minmax(b_copy,3,INT_MIN,INT_MAX);
    //         if (x<opt_val){
    //             opt_val=x;
    //             opt_move=i;
    //         }
    //     }

    // }
    this->best_move = moves[opt_move];
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Output the elapsed time
    std::cout << "Time taken by function: " << duration.count() << " seconds" << std::endl;
    // if (moveset.size() == 0) {
    //     this->best_move = 0;
    // }
    // else {
    //     std::vector<U16> moves;
    //     std::cout << all_boards_to_str(b) << std::endl;
    //     for (auto m : moveset) {
    //         std::cout << move_to_str(m) << " ";
    //     }
    //     std::cout << std::endl;
    //     std::sample(
    //         moveset.begin(),
    //         moveset.end(),
    //         std::back_inserter(moves),
    //         1,
    //         std::mt19937{std::random_device{}()}
    //     );
    //     this->best_move = moves[0];
    // }

}
