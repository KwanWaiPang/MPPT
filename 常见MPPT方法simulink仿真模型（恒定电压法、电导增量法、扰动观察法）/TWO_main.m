for S=400:200:1000
    T=25;
sim('TWO.slx')
figure(1)
plot(V,P,'DisplayName',['S=',num2str(S)])
hold on
axis([0 50 0 400])
xlabel('U/W')
ylabel('P/W')
title('光照变化P-U特性曲线')
legend

figure(2)
plot(V,I,'DisplayName',['S=',num2str(S)])
hold on
axis([0 50 0 20])
xlabel('U/W')
ylabel('I/A')
title('光照变化I-U特性曲线')
legend
end


for T=25:15:70
    S=1000;
sim('TWO.slx')
figure(3)
plot(V,P,'DisplayName',['T=',num2str(T)])
hold on
axis([0 50 0 400])
xlabel('U/W')
ylabel('P/W')
title('温度变化P-U特性曲线')
legend

figure(4)
plot(V,I,'DisplayName',['T=',num2str(T)])
hold on
axis([0 50 0 20])
xlabel('U/W')
ylabel('I/A')
title('温度变化I-U特性曲线')
legend
end