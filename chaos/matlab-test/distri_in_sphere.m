R = 100;
count = 5000;
x = zeros(1, count);
y = zeros(1, count);
z = zeros(1, count);

tic;

for i = 1 : count
    theta = rand * 2 * pi;
    phi = acos(1 - 2 * rand);
    r = R * nthroot(rand, 3);
    x(i) = r * sin(phi) * cos(theta);
    y(i) = r * sin(phi) * sin(theta);
    z(i) = r * cos(phi);
end

plot3(x, y, z, 'ro', 'MarkerSize',2, 'MarkerFaceColor','r');
axis equal;
toc;

