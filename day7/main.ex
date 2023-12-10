defmodule Solution do
  defp hand_type(hand) do
    case hand do
      [5] -> :Five_of_kind
      [4, 1] -> :Four_of_kind
      [3, 2] -> :Full_house
      [3, 1, 1] -> :Three_of_kind
      [2, 2, 1] -> :Two_pair
      [2, 1, 1, 1] -> :One_pair
      [1, 1, 1, 1, 1] -> :High_card
      _ -> :error
    end
  end

  defp type_order(type) do
    case type do
      :Five_of_kind -> 10
      :Four_of_kind -> 9
      :Full_house -> 8
      :Three_of_kind -> 7
      :Two_pair -> 6
      :One_pair -> 5
      :High_card -> 4
      _ -> IO.puts("Error") 
          :error
    end
  end
  
  defp hand_cmp(h1, h2) do # [12, 10, ...], [8, 4, ...] -> h1 < h2
    t1 = h1
      |> Enum.frequencies()
      |> Enum.reduce( [], fn({card, freq}, acc) -> 
        [[card, freq] | acc] end)
      |> Enum.sort(fn([_, f1], [_, f2]) -> f1 > f2 end)
      |> Enum.map(fn([_, f1]) -> f1 end)
      |> hand_type()

    t2 = h2
      |> Enum.frequencies()
      |> Enum.reduce( [], fn({card, freq}, acc) -> 
        [[card, freq] | acc] end)
      |> Enum.sort(fn([_, f1], [_, f2]) -> f1 > f2 end)
      |> Enum.map(fn([_, f1]) -> f1 end)
      |> hand_type()

    if type_order(t1) == type_order(t2) do
      h1|> Enum.zip(h2)
        |> Enum.all?( fn {c1, c2} -> c1 <= c2 end )
    else
      type_order(t1) <= type_order(t2)
    end
  end
  
  defp parse_rank("T"), do: 10
  defp parse_rank("J"), do: 11
  defp parse_rank("Q"), do: 12
  defp parse_rank("K"), do: 13
  defp parse_rank("A"), do: 14
  defp parse_rank(_), do: :error
  defp parse_hand(hand) do
    hand 
      |> String.graphemes()
      |> Enum.map( &case Integer.parse(&1) do
          {d, _} -> d
          :error -> parse_rank(&1)
          _ -> :error
          end )
  end

  def tmp(x) do
    x |> Enum.frequencies()
      |> Enum.reduce( [], fn({card, freq}, acc) -> 
        [[card, freq] | acc] end)
      |> Enum.sort(fn([_, f1], [_, f2]) -> f1 > f2 end)
      |> Enum.map(fn([_, f1]) -> f1 end)
      |> hand_type()
  end

  def part1(content) do
    tmp = content
      |> String.split("\n")
      |> Enum.filter( &( &1 != "" ) )
      |> Enum.map( &(&1 |> String.split(" ")) )
      |> Enum.map( &(
        [ &1 |> Enum.at(0) |> parse_hand(),
          &1 |> Enum.at(1) |> Integer.parse() |> elem(0) ]
      ))
      |> Enum.sort(fn([c1, _], [c2, _]) -> hand_cmp(c1, c2) end)

    # tmp |> Enum.map( fn [cards, _] -> tmp(cards) end )
    #     |> IO.inspect()

    tmp |> Enum.with_index(1)
      |> Enum.map(fn {[_, bet], i} -> bet * i end )
      |> Enum.sum()
      |> IO.inspect()
  end
  def part2(content) do
  end
end

case File.read "puzzle.txt" do
# case File.read "example.txt" do
  {:ok, content} ->
    # IO.puts(content)
    Solution.part1(content)
    Solution.part2(content)
  {:error} -> IO.puts("Failed to read puzzle.txt")
end
