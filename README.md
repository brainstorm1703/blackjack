1.	Implement card game blackjack;
2.	For rendering use SDL2 (https://www.libsdl.org/) or Qt (Qt is preferable);
3.	Cards must appear with animation (moving from any side of screen would be enough);
4.	Must be implemented mechanism for adding new skins for cards (backs and front textures) (hint: you can create texture from image);
5.	Implement possibility to choose skin in runtime;
Note: Must be present only buttons for getting additional card(Hit) or stop(Stand). Dealer’s and player’s cards must be rendered.
6.	User has some starting balance and can increase/decrease bet value, but it can`t be zero. Game should be finished if user doesn’t have enough money to make a bet.
7.	Rules:
  a)	The goal of blackjack is to beat the dealer's hand without going over 21.
  b)	Face cards(king, for example) are worth 10. Aces are worth 1 or 11, whichever makes a better hand. Card`s with number value equals to its number.
  c)	Each player starts with two cards, one of the dealer's cards is hidden(card’s back is rendered) until the end.
  d)	To 'Hit' is to ask for another card. To 'Stand' is to hold your total and end your turn.
  e)	If you go over 21 you bust, and the dealer wins regardless of the dealer's hand.
8.	Implement in C++ side some sound playback (some background music, buttons press sound, card sound).
Implementation should contain modern C++ features and best practices. As example virtual functions, smart pointers, RAII and so on. You are free to choose any C++ standard and use its features. 
Version control system usage is mandatory. 
