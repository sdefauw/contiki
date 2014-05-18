close all;

%% data
number_of_note_start = [2 3 4 5 6 7 8 9 10 11 12 13 14 15 ];
time_start = [48656 105660 208231 142811 202021 316981 391981 557611 603528 539137 622300 739937 771051 937124 ];
msg_ip_start = [7, 1, 0, 0 ; 31, 1, 0, 0 ; 73, 1, 0, 0 ; 99, 1, 0, 0 ; 167, 1, 0, 0 ; 250, 1, 0, 0 ; 376, 1, 0, 0 ; 551, 1, 0, 0 ; 714, 1, 0, 0 ; 657, 1, 0, 0 ; 980, 1, 0, 0 ; 1155, 1, 0, 0 ; 1274, 1, 0, 0 ; 1574, 1, 0, 0 ];
msg_nd6_start = [4, 5 ; 11, 11 ; 19, 19 ; 29, 26 ; 42, 35 ; 57, 45 ; 80, 63 ; 106, 84 ; 128, 100 ; 141, 101 ; 188, 138 ; 217, 158 ; 250, 177 ; 299, 211 ];

number_of_note_hour = [2 3 4 5 6 7 8 9 10 11 12 13 14 15 ];
time_hour = [3648660 3705668 3808241 3742823 3802036 3916999 3992001 4157634 4203553 4139165 4222331 4339971 4371087 4537165 ];
msg_ip_hour = [54, 16, 0, 0 ; 141, 2, 0, 0 ; 291, 2, 0, 0 ; 447, 2, 0, 0 ; 668, 2, 0, 0 ; 896, 2, 0, 0 ; 2238, 2, 0, 0 ; 1511, 2, 0, 0 ; 2331, 2, 0, 0 ; 5005, 2, 0, 0 ; 2355, 2, 0, 0 ; 6181, 2, 0, 0 ; 9019, 2, 0, 0 ; 8055, 2, 0, 0 ];
msg_nd6_hour = [46, 47 ; 67, 67 ; 100, 100 ; 122, 119 ; 156, 149 ; 188, 176 ; 311, 287 ; 275, 253 ; 360, 332 ; 534, 468 ; 405, 358 ; 733, 651 ; 855, 744 ; 894, 780 ];
% enddata

number_of_note = number_of_note_start;

time_dif = time_hour-time_start;
msg_ip_dif = msg_ip_hour-msg_ip_start;
msg_nd6_dif = msg_nd6_hour-msg_nd6_start;


%% Graph Starting
% Timing
figure;
plot(number_of_note, time_start./1000,'*-');
ylabel('Convergence time (s)');
xlabel('Number of nodes');
title('Convergence time of the network (line topology) - starting');

% Msg IP
figure;
plot(number_of_note, msg_ip_start,'*-');
legend('recv','sent','forwarded', 'drop',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of IP messages (line topology) - starting');

% Msg ND6
figure;
plot(number_of_note, msg_nd6_start,'*-');
legend('recv','sent',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of ND6 messages (line topology) - starting');


%% Graph 1hour
% Timing
% figure;
% plot(number_of_note, time_dif,'*-');
% ylabel('Convergence time (ms)');
% xlabel('Number of nodes');
% title('Convergence time of the network (line topology) - 1 hour');

% Msg IP
figure;
plot(number_of_note, msg_ip_dif,'*-');
legend('recv','sent','forwarded', 'drop',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of IP messages (line topology) - 1 hour');

% Msg ND6
figure;
plot(number_of_note, msg_nd6_dif,'*-');
legend('recv','sent',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of ND6 messages (line topology) - 1 hour');

%% Graph All
% Timing
% figure;
% plot(number_of_note, time_hour,'*-');
% ylabel('Convergence time (ms)');
% xlabel('Number of nodes');
% title('Convergence time of the network (line topology) - All');

% Msg IP
figure;
plot(number_of_note, msg_ip_hour,'*-');
legend('recv','sent','forwarded', 'drop',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of IP messages (line topology) - All');

% Msg ND6
figure;
plot(number_of_note, msg_nd6_hour,'*-');
legend('recv','sent',2);
ylabel('Number of messages');
xlabel('Number of nodes');
title('Number of ND6 messages (line topology) - All');
