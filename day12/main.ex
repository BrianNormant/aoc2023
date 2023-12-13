defmodule Solution do
  def parse(l) do
    l |> String.split(" ", trim: true)
      |> then( fn [sprgs, rcrd] ->
          rcrd = rcrd
            |> String.split(",", trim: true)
            |> Enum.map( &case Integer.parse(&1) do
              {d, _} -> d
              _ -> :error
            end)
          sprgs = sprgs
            |> String.graphemes()
            |> Enum.map( &case &1 do 
              "#" -> :d
              "." -> :s
              "?" -> :u
              _ -> :error
            end)
          {sprgs, rcrd}
      end)
  end

  # generate all possible arangments.
  defp arangments(ar, []), do: ar
  defp arangments(ar, [c | r]) do
    cached = Process.get([c | r])
    case cached do
    nil ->
      cached = case c do
          :d -> arangments( Enum.map(ar, &( Tuple.append(&1, :d) )), r )
          :s -> arangments( Enum.map(ar, &( Tuple.append(&1, :s) )), r )
          :u ->
            ar = ar
              |> Enum.map( &([ Tuple.append(&1, :s), Tuple.append(&1, :d) ]) )
              |> List.flatten()
            arangments(ar, r)
          end
        Process.put([c | r], cached)
        cached
      _ -> cached
    end
  end

  def compute({s, r}) do
    s = arangments([{}], s)
      |> Enum.map( &Tuple.to_list/1 )
    Enum.filter(s, fn l ->
        l |> Enum.chunk_by( &Function.identity/1 )
          |> Enum.filter( &Enum.all?(&1, fn b -> b == :d end) )
          |> Enum.map( &Enum.count/1 )
          |> then( &( &1 == r) )
    end)
  end

  def part1(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map(fn l ->
          l |> parse()
            |> compute()
            |> Enum.count()
        end)
      |> Enum.sum()
      |> IO.inspect()
  end
  def part2(c) do
    c |> String.split("\n", trim: true)
      |> Enum.map(fn l ->
          l |> parse()
            |> then(fn {a, b} ->
              {
                a |> List.duplicate(5) |> List.flatten(),
                b |> List.duplicate(5) |> List.flatten()
              }
              end )
            |> compute()
            |> Enum.count()
        end)
      |> Enum.sum()
      |> IO.inspect()
  end
end

case File.read("example.txt") do
# case File.read("puzzle.txt") do
  {:ok, content} ->
    Solution.part1(content)
    Solution.part2(content)
  {:error, _} -> IO.put("Failed to read example.txt")
end
