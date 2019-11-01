u = [600, 100];
v = [100, 500];

tic;

for i = 1 : 5000
    a = rand;
    b = rand;
    x(i) = a * u(1) + b * v(1);
    y(i) = a * u(2) + b * v(2);
end

plot(x, y, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');

toc;
