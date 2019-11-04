R = 100;
count = 5000;
x = zeros(1, count);
y = zeros(1, count);

tic;

for i = 1 : count
    theta = rand * 2 * pi;
    r = R * sqrt(rand);
    x(i) = r * cos(theta);
    y(i) = r * sin(theta);
end

plot(x, y, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');

toc;