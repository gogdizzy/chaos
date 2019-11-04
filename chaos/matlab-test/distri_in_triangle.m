
A = [100, 100];
B = [500, 100];
C = [700, 400];

hold on;

plot(A(1), A(2), 'g.', 'MarkerSize', 10);
plot(B(1), B(2), 'g.', 'MarkerSize', 10);
plot(C(1), C(2), 'g.', 'MarkerSize', 10);


u = B - A;
v = C - A;
w = C - B;
count = 5000;
x = zeros(1, count);
y = zeros(1, count);
dir = sign(u(1) * v(2) - u(2) * v(1))

tic;

for i = 1 : count
    a = rand;
    b = rand;
    x(i) = a * u(1) + b * v(1) + A(1);
    y(i) = a * u(2) + b * v(2) + A(2);
    w2 = [x(i), y(i)] - C;
    if (w(1) * w2(2) - w(2) * w2(1)) * dir < 0
        x(i) = B(1) + C(1) - x(i);
        y(i) = B(2) + C(2) - y(i);
    end
end

plot(x, y, 'r.', 'MarkerSize', 5);
axis equal;

toc;

