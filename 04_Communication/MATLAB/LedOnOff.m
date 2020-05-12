s = serial('/dev/tty.usbserial-DN0421L4', 'Baudrate', 9600);
fopen(s);
for i = 1:5
    fprintf('Turnning LED ON\n');
    fprintf(s, 'ON');
    pause(2);
    fprintf('Turnning LED OFF\n');
    fprintf(s, 'OFF');
    pause(2);
end
fclose(s);
fprintf('Done!\n');