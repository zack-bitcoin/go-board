function num2letter(x){
    return(substr(Letters, x, 1))
}
function letter2number(x){
    match(Letters, x)
    return(RSTART)
}
function new_game(handicap){
    system("(gnugo --mode gtp --gtp-listen 8070)&")
    system("sleep 0.01")
    Service = "nc 127.0.0.1 8070"
    command("fixed_handicap " handicap, Service)
    white_play(Service)
    return(Service)
}
function command(text, Service,        s){
    print(text) |& Service
    (Service |& getline s)
    while((Service |& getline result) > 0){
        if(length(result) == 0){return(s)}
        s = s "\n" result
    }
}
function black_play(x, Service){
    y = command("black " x, Service)
    update_board(Service)
    return(y)
}
function white_play(Service,      x){
    x = command("genmove_white\n", Service)
    print(x) > recent_move_file
    system("sleep 1")
    update_board(Service)
}
function play2(x, y){
    x = num2letter(x)
    try_play(x 20-y, Service)
}
function try_play(x, Service){
    y = black_play(x, Service)
    if(y == "? illegal move"){
        print("illegal move")
    } else if(y == "= "){
        white_play(Service)
    } else {
        print("failed in try_play: " x " : " y)
    }
}
function update_board(Service,     x){
    x = command("showboard\n", Service)
    draw_board(x)
}
function quit(Service){
    print("quit")
    x = command("quit\n", Service)
    close(Service)
}
function draw_board(t){
    s = "convert "
    M = split(t, X, "\n")
    for(j=3; j<=M-1; j++){
        x = substr(X[j], 4, 38)
        N = split(x, A, " ")
        s = s " \\( "
        for(i=1; i<= N; i++){
            if(A[i] == "."){
                s = s " images/empty.MIFF "
            }
            if(A[i] == "+"){
                s = s " images/star.MIFF "
            }
            if(A[i] == "O"){
                s = s " images/white.MIFF "
            }
            if(A[i] == "X"){
                s = s " images/black.MIFF "
            }
        }
        s = s " +append \\) "
    }
    s = s " -background none -append board.MIFF"
    system(s)
    if((getline line < recent_move_file) > 0){
        l2a = letter2number(substr(line, 3, 1))
        l2b = substr(line, 4)
        system("composite -geometry  +"((l2a-1)*39)"+" (39*(19-l2b))" images/mark.MIFF board.MIFF board.MIFF")
        system("rm " recent_move_file)
    } else {
    }
    close(recent_move_file)
    system("convert board.MIFF board2.png")
    system("mv board2.png board.png")
}
function mainloop(Service,      file, x, y){
    file = "coordinates"
    system("touch " file)
    if ((getline line < file) > 0){
        split(line, A, ", ")
        if(A[1] == "quit"){
            quit(Service)
            close(file)
            system("rm " file)
            return(0)
        } else if(A[1] == "undo"){
            command("undo\n", Service)
            command("undo\n", Service)
            update_board(Service)
        } else {
            x = int(A[1] / 39)+1
            y = int(A[2] / 39)+1
            play2(x, y)
        }
        close(file)
        system("rm " file)
    } else {
        close(file)
    }
    system("sleep 0.1")
    mainloop(Service)
}
BEGIN{
    Letters =  "ABCDEFGHJKLMNOPQRST"
    recent_move_file = "recent_move"
    handicap = 5
    Service = new_game(handicap)
    update_board(Service)
    system("(./board)&")
    mainloop(Service)
}
