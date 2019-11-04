a = 200;
b = 150;
k = b / a;
count = 5000;
x = zeros(1, count);
y = zeros(1, count);

tic;

for i = 1 : count
    theta = rand * 2 * pi;
    r = sqrt(rand) * a;
    x(i) = r * cos(theta);
    y(i) = k * r * sin(theta);
end

plot(x, y, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');
axis equal;

toc;