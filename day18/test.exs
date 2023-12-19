defmodule Solution do
  def f(a) do
    [f | _] = a
    a = a ++ [f]
    a |> Enum.chunk_every(2, 1, :discard)
  end
end

list = [1,2,3,4,5]
Solution.f(list) 
 |> IO.inspect()
