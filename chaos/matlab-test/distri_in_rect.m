xrange = [100, 600];
yrange = [100, 500];
count = 5000;

tic;

x = randi(xrange, 1, count);
y = randi(yrange, 1, count);

plot(x, y, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');
axis equal;

toc;