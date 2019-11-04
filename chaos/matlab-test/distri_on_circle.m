R = 100;
count = 1000;
x = zeros(1, count);
y = zeros(1, count);

tic;

for i = 1 : count
    theta = rand * 2 * pi;
    x(i) = R * cos(theta);
    y(i) = R * sin(theta);
end

plot(x, y, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');

toc;